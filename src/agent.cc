// agent.cc
// Jose Maria Martinez
// Implementation of the agent. Movement functionality and decision making
// functions.
//Comments for the functions can be found at the header

#include <cmath>
#include "agent.h"
#include "gamestate.h"
#include "path_finder.h"

static u32 total_agents = 1;

Agent::Agent() : type_agent_(AgentType::k_Small)
{
  init(0, 0);
}

Agent::Agent(const AgentType agent_type, const float x, const float y, PathFinder* pf) : type_agent_(agent_type)
{
  init(x, y);
  path_finder_agent_ = pf;
}

Agent::Agent(const AgentType agent_type, const float x, const float y) : type_agent_(agent_type)
{
  init(x, y);
}


Agent::~Agent()
{
  if(path_)
  {
    delete path_;
    path_ = nullptr;
  }
  if(representation_)
  {
    ESAT::SpriteRelease(representation_);
  }
}

void Agent::init(const float x, const float y)
{
  position_ = Float2(x, y);
  velocity_ = Float2(0, 0);
  target_position_ = position_;
  id_ = total_agents;
  total_agents++;
  initialized_ = false;
  representation_ = nullptr;
  path_ = new Path();
}

ESAT::SpriteHandle Agent::representation() const
{
  return representation_;
}

void Agent::update(const u32 dt)
{
  updateMind(dt);
  updateBody(dt);
}

float Agent::x() const
{
  return position_.x;
}

float Agent::y() const
{
  return position_.y;
}


void Agent::updateBody(const u32 dt)
{
  switch (move_type_)
  {
  case MovementType::k_MovDeterminist:
    MOV_Determinist(dt);
    break;
  case MovementType::k_MovRandom:
    MOV_Random(dt);
    break;
  case MovementType::k_MovTracking:
    MOV_Tracking(dt);
    break;
  case MovementType::k_MovPattern:
    MOV_Pattern(dt);
    break;
  case MovementType::k_MovStop:
    MOV_Stop();
    break;
  case MovementType::k_MovAStar:
    MOV_AStar(dt);
  default:
    break;
  }
  calculateVelocity();
  move(dt);
}

void Agent::updateMind(const u32 dt)
{
  mind_acum_ += dt;
  if (mind_acum_ < mind_time_) return;

  mind_acum_ = 0;  
  if (!initialized_)
  {
    mail_box_ = static_cast<AgentMessage*>(malloc(total_agents * sizeof(AgentMessage)));
    for (u32 i = 0; i < total_agents; i++)
    {
      (mail_box_ + i)->type = AgentMessageType::k_Nothing;
      (mail_box_ + i)->position = Float2(0.0f, 0.0f);
    }
    objective_ = nullptr;
    actual_state_ = FSMStates::k_Working;
    const float generic_speed = 5.0f;
    //const float generic_speed = 50.0f;
    speed_ = generic_speed;
    switch (type_agent_)
    {
    case AgentType::k_Hero:
      move_type_ = MovementType::k_MovStop;
      speed_ *= 20.0f;
      epsilon_ = kEpsilonFactor * speed_;
      //mind_time_ = 1000;
      representation_ = ESAT::SpriteFromFile("../../../data/gfx/agents/allied_soldier.bmp");
      break;
    case AgentType::k_Huge:
      ////
      path_->create(2);
      path_->addPoint(1000.0f, 0.0f);
      path_->addPoint(0.0f, 0.0f);
      path_->set_direction(Direction::kDirForward);
      path_->set_action(Action::kActionLoopInfinite);
      path_->setToReady();
      //
      move_type_ = MovementType::k_MovDeterminist;

      tracking_retarget_time_ = 3000; //3s

      speed_ *= 0.75f;
      epsilon_ = kEpsilonFactor * speed_;
      representation_ = ESAT::SpriteFromFile("../../../data/gfx/agents/big_agent.png");
      break;
    case AgentType::k_Normal:
      move_type_ = MovementType::k_MovRandom;
      next_random_time_ = 5000; //5s
      accum_time_random_ = 0;

      tracking_retarget_time_ = 1500; //1.5s

      speed_ *= 1.0f;
      epsilon_ = kEpsilonFactor * speed_;
      representation_ = ESAT::SpriteFromFile("../../../data/gfx/agents/normal_agent.png");
      break;
    case AgentType::k_Small:
      move_type_ = MovementType::k_MovPattern;
      pattern_idx_ = 0;
      pattern_targets_[0] = PatternCommand{ PatternToken::k_East, 3000 };
      pattern_targets_[1] = PatternCommand{ PatternToken::k_South, 3000 };
      pattern_targets_[2] = PatternCommand{ PatternToken::k_West, 3000 };
      pattern_targets_[3] = PatternCommand{ PatternToken::k_North, 3000 };

      tracking_retarget_time_ = 1000; //1s

      accum_time_pattern_ = 0;
      pattern_step_ = 50;
      speed_ *= 1.25f;
      epsilon_ = kEpsilonFactor * speed_;
      representation_ = ESAT::SpriteFromFile("../../../data/gfx/agents/small_agent.png");
      break;
    default:
      break;
    }

    initialized_ = true;
  }

  for (u32 i = 0; i < total_agents; i++)
  {
    if ((mail_box_ + i)->type == AgentMessageType::k_PathIsReady)
    {
      move_type_ = MovementType::k_MovAStar;
      target_reached_ = true;
      (mail_box_ + i)->type = AgentMessageType::k_Nothing;
      break;
    }
    (mail_box_ + i)->position = Float2(0.0f, 0.0f);
  }

  switch (actual_state_) {
  case FSMStates::k_Working:
    FSM_Working(dt);
    break;
  case FSMStates::k_Chasing:
    FSM_Chasing(dt);
    break;
  case FSMStates::k_Fleeing:
    FSM_Fleeing(dt);
    break;
  case FSMStates::k_Resting:
    FSM_Resting(dt);
    break;
  default:
    break;
  }

}

void Agent::move(const u32 dt)
{
  //displacement
  const float time_in_seconds = dt * 0.001f;
  Float2 e = velocity_ * time_in_seconds;
  position_ += e;
}

bool Agent::positionReached() const
{
  Float2 aux_vector = target_position_ - position_;
  return aux_vector.Length() < epsilon_;
}

void Agent::setNextPosition(float new_target_x, float new_target_y)
{
  target_position_ = Float2(new_target_x, new_target_y);
}

void Agent::setOrientation(const Float2& destination)
{
  velocity_ = destination - position_;
  // 0/0 exception
  if (velocity_.Length() == 0.0f) return;
  velocity_ /= velocity_.Length();
}

void Agent::calculateVelocity()
{
  setOrientation(target_position_);
  velocity_ *= speed_;
}

void Agent::FSM_Working(u32 dt) {
#ifdef DEBUG
  accum_time_ += dt;
#endif
  

  for (Agent* agent : GameState::instance().agents_)
  {
    Float2 distance = agent->position_ - this->position_;
    const float d = distance.Length();
#ifdef DEBUG
    if (accum_time_ >= time_for_print_)
    {
      printf("I'm at a distance of: %f \n", d);
      accum_time_ = 0;
    }
#endif
    //We check if the agent is in chase range and we are bigger
    if(agent != this && isBigger(agent) && d <= chase_distance_)
    {
#ifdef DEBUG
      printf("Changing to chase \n");
#endif
      actual_state_ = FSMStates::k_Chasing;
      objective_ = agent;
      move_type_ = MovementType::k_MovTracking;
      return;
    }
    //We check if the agent is in flee range and is smaller
    if(agent != this && isSmaller(agent) && d <= flee_distance_)
    {
#ifdef DEBUG
      printf("Changing to flee \n");
#endif
      actual_state_ = FSMStates::k_Fleeing;
      objective_ = agent;
      move_type_ = MovementType::k_MovTracking;
      return;
    }
  }
}

void Agent::FSM_Chasing(u32 dt) {
  Float2 distance = objective_->position_ - this->position_;
  const float d = distance.Length();

#ifdef DEBUG
  accum_time_ += dt;
  if (accum_time_ >= time_for_print_)
  {
    printf("I'm at a distance of: %f \n", d);
    accum_time_ = 0;
  }
#endif

  if(d >= lost_focus_distance_)
  {
#ifdef DEBUG
    printf("I'm going to take a rest \n");
#endif
    actual_state_ = FSMStates::k_Resting;
    move_type_ = MovementType::k_MovStop;
    objective_ = nullptr;
    return;
  }
  if(target_reached_)
  {
#ifdef DEBUG
    printf("I'm chasing to: {%f,%f} \n", objective_->position_.x, objective_->position_.y);
#endif
    target_position_ = objective_->position_;
    target_reached_ = false;
  }
}

void Agent::FSM_Fleeing(u32 dt) {
  Float2 distance = this->position_ - objective_->position_;
  //Float2 distance = objective_->position_ - this->position_;
  const float d = distance.Length();
#ifdef DEBUG
  accum_time_ += dt;
  if (accum_time_ >= time_for_print_)
  {
    printf("I'm at a distance of: %f \n", d);
    accum_time_ = 0;
  }
#endif
  if (d >= lost_focus_distance_)
  {
#ifdef DEBUG
    printf("I'm going to take a rest \n");
#endif
    actual_state_ = FSMStates::k_Resting;
    move_type_ = MovementType::k_MovStop;
    objective_ = nullptr;
    return;
  }
  if (target_reached_)
  {
    Float2 direction = distance / d;
    direction *= 100.0f;
    Float2 final_position = position_ + direction;
    //We normalize to our screen boundaries they are not exact because the
    //sprites of the agents are not exactly a pixel, so they get out.
    final_position.x = fmax(final_position.x, 20.0f);
    final_position.y = fmax(final_position.y, 20.0f);
    final_position.x = fmin(final_position.x, 1260.0f);
    final_position.y = fmin(final_position.y, 680.0f);

    target_position_ = final_position;
    target_reached_ = false;
#ifdef DEBUG
    printf("I'm going to flee to: {%f,%f} \n", final_position.x, final_position.y);
#endif
  }
}

void Agent::FSM_Resting(const u32 dt) {
  time_rested_ += dt;
  if(time_rested_ >= resting_time_)
  {
#ifdef DEBUG
    printf("Time to work a bit. \n");
#endif
    time_rested_ = 0;
    actual_state_ = FSMStates::k_Working;
    switch(type_agent_)
    {
    case AgentType::k_Small:
      move_type_ = MovementType::k_MovPattern;
      break;
    case AgentType::k_Normal:
      move_type_ = MovementType::k_MovRandom;
      break;
    case AgentType::k_Huge:
      move_type_ = MovementType::k_MovDeterminist;
      break;
    default:
      move_type_ = MovementType::k_MovAStar;
      break;
    }
  }

  for (Agent* agent : GameState::instance().agents_)
  {
    Float2 distance = agent->position_ - this->position_;
    const float d = distance.Length();
    //We check if the agent is in flee range and is smaller
    if (agent != this && isSmaller(agent) && d <= flee_distance_)
    {
      actual_state_ = FSMStates::k_Fleeing;
      objective_ = agent;
      move_type_ = MovementType::k_MovTracking;
      return;
    }
  }
}

bool Agent::isBigger(Agent* a)
{
  //If we are huge and the other agent is not huge, we are bigger
  bool bigger_if_huge = ((type_agent_ == AgentType::k_Huge) &&
                        (a->type_agent_ != AgentType::k_Huge));

  //If we are medium and the other agent is small, we are bigger
  bool bigger_if_medium = ((type_agent_ == AgentType::k_Normal) &&
                          (a->type_agent_ == AgentType::k_Small));

  //If one of the previous conditions is true we are bigger
  return bigger_if_huge || bigger_if_medium;
}

bool Agent::isSmaller(Agent* a)
{
  //If we are small and the other agent is not small, we are smaller
  bool smaller_if_small = (type_agent_ == AgentType::k_Small) &&
                          (a->type_agent_ != AgentType::k_Small);

  //If we are normal and the other agent is huge, we are smaller
  bool smaller_if_normal = (type_agent_ == AgentType::k_Normal) &&
    (a->type_agent_ == AgentType::k_Huge);

  //If one of the previous conditions is true we are bigger
  return smaller_if_small || smaller_if_normal;
}



void Agent::MOV_Determinist(const u32 dt)
{
  if (!positionReached()) return;
  target_reached_ = true;

  const Float2* p = path_->nextPoint();
  setNextPosition(p->x, p->y);
}

void Agent::MOV_Random(const u32 dt)
{

  accum_time_random_ += dt;

  if (accum_time_random_ < next_random_time_ && !positionReached()) return;
  static float const boundary_x_left = 0.0f;
  static float const boundary_x_right = 1280.0f;
  static float const boundary_y_left = 0.0f;
  static float const boundary_y_right = 720.0f;

  //float rand_px = rand() % 1280;
  //float rand_py = rand() % 720;
  float rand_px = rand() / float(RAND_MAX);
  float rand_py = rand() / float(RAND_MAX);
  rand_px = rand_px * boundary_x_right + boundary_x_left;
  rand_py = rand_py * boundary_y_right + boundary_y_left;

  setNextPosition(rand_px, rand_py);

  accum_time_random_ = 0;
}

void Agent::MOV_Tracking(const u32 dt)
{
  accum_time_tracking_ += dt;
  if (accum_time_tracking_ < tracking_retarget_time_ && !positionReached()) return;
  target_reached_ = true;
  setNextPosition(target_position_.x, target_position_.y);
  accum_time_tracking_ = 0;
}

void Agent::MOV_Pattern(const u32 dt)
{
  accum_time_pattern_ += dt;
  if (accum_time_pattern_ > pattern_targets_[pattern_idx_].seconds)
  {
    accum_time_pattern_ = 0;
    pattern_idx_ = (pattern_idx_ + 1) % pattern_size_;
  }
  switch (pattern_targets_[pattern_idx_].token)
  {
  case PatternToken::k_East:
    target_position_ = Float2(position_.x + pattern_step_, position_.y);
    break;
  case PatternToken::k_West:
    target_position_ = Float2(position_.x - pattern_step_, position_.y);
    break;
  case PatternToken::k_North:
    target_position_ = Float2(position_.x, position_.y - pattern_step_);
    break;
  case PatternToken::k_South:
    target_position_ = Float2(position_.x, position_.y + pattern_step_);
    break;
  default:
    break;
  }
}

void Agent::MOV_Stop()
{
  target_position_ = position_;
}

void Agent::MOV_AStar(const u32 dt) {
  if (target_reached_)
  {
    if (path_->isLast())
    {
      move_type_ = MovementType::k_MovStop;
    }else
    {
      if (!positionReached()) return;
      target_reached_ = true;
      const Float2* p = path_->nextPoint();
      setNextPosition(p->x, p->y);
    }
  }
}

void Agent::prepareAStarMessage(const Float2& origin, const Float2& dst)
{
  if(path_finder_agent_ && !path_->isReady())
  {
    AgentMessage msg;
    msg.type = AgentMessageType::k_AskForPath;
    msg.position = origin;
    msg.dst = dst;
    msg.path = path_;
    path_finder_agent_->sendMessage(msg, id_);
    //path_finder_agent_->generatePath(&path_, origin, dst);
  }
}

void Agent::prepareAStar(const Float2& origin, const Float2& dst)
{
  //if (path_->isReady()) return;
  path_finder_agent_->generatePath(path_, origin, dst);
}

void Agent::startAStar()
{
  target_reached_ = true;
  move_type_ = MovementType::k_MovAStar;
}

void Agent::sendMessage(const AgentMessage& msg, const u32 id)
{
  *(mail_box_ + id) = msg;
}