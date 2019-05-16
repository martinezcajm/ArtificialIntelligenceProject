// agent.cc
// Jose Maria Martinez
// Implementation of the agent. Movement functionality and decision making
// functions.
//Comments for the functions can be found at the header

#include <cmath>
#include "agent.h"
#include "gamestate.h"
#include "path_finder.h"

static uint32_t total_agents = 0;

Agent::Agent() : type_agent_(AgentType::k_Mindless)
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

}

void Agent::init(const float x, const float y)
{
  position_ = Float2(x, y);
  velocity_ = Float2(0, 0);
  target_position_ = position_;
  id_ = total_agents;
  total_agents++;
  initialized_ = false;
}

void Agent::update(const uint32_t dt)
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


void Agent::updateBody(const uint32_t dt)
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

void Agent::updateMind(const uint32_t dt)
{
  mind_acum_ += dt;
  if (mind_acum_ < mind_time_) return;

  mind_acum_ = 0;
  if (!initialized_)
  {
    actual_state_ = FSMStates::k_Working;
    const float generic_speed = 5.0f;
    speed_ = generic_speed;
    switch (type_agent_)
    {
    case AgentType::k_Mindless:
      ////
      path_.create(2);
      path_.addPoint(1000.0f, 0.0f);
      path_.addPoint(0.0f, 0.0f);
      path_.set_direction(Direction::kDirForward);
      path_.set_action(Action::kActionLoopInfinite);
      path_.setToReady();
      ////
      move_type_ = MovementType::k_MovDeterminist;

      speed_ *= 0.75f;
      epsilon_ = kEpsilonFactor * speed_;
      break;
    case AgentType::k_Scout:
      move_type_ = MovementType::k_MovRandom;
      next_random_time_ = 5000; //5s
      accum_time_random_ = 0;

      speed_ *= 1.0f;
      epsilon_ = kEpsilonFactor * speed_;
      break;
    case AgentType::k_Chaser:
      move_type_ = MovementType::k_MovTracking;
      tracking_retarget_time_ = 3000; //3s
      accum_time_tracking_ = 0;

      speed_ *= 1.25f;
      epsilon_ = kEpsilonFactor * speed_;
      target_position_ = Float2(640, 360);
      break;
    case AgentType::k_Patrol:
      move_type_ = MovementType::k_MovPattern;
      pattern_idx_ = 0;
      pattern_targets_[0] = PatternCommand{ PatternToken::k_East, 3000 };
      pattern_targets_[1] = PatternCommand{ PatternToken::k_South, 3000 };
      pattern_targets_[2] = PatternCommand{ PatternToken::k_West, 3000 };
      pattern_targets_[3] = PatternCommand{ PatternToken::k_North, 3000 };

      accum_time_pattern_ = 0;
      pattern_step_ = 50;
      speed_ *= 1.50f;
      epsilon_ = kEpsilonFactor * speed_;
      break;
    case AgentType::k_Hero:
      move_type_ = MovementType::k_MovStop;
      speed_ *= 20.0f;
      epsilon_ = kEpsilonFactor * speed_;
      break;
    case AgentType::k_Huge:
      ////
      path_.create(2);
      path_.addPoint(1000.0f, 0.0f);
      path_.addPoint(0.0f, 0.0f);
      path_.set_direction(Direction::kDirForward);
      path_.set_action(Action::kActionLoopInfinite);
      path_.setToReady();
      ////
      move_type_ = MovementType::k_MovDeterminist;

      speed_ *= 0.75f;
      epsilon_ = kEpsilonFactor * speed_;
      break;
    case AgentType::k_Normal:
      move_type_ = MovementType::k_MovRandom;
      next_random_time_ = 5000; //5s
      accum_time_random_ = 0;

      speed_ *= 1.0f;
      epsilon_ = kEpsilonFactor * speed_;
      break;
    case AgentType::k_Small:
      move_type_ = MovementType::k_MovPattern;
      pattern_idx_ = 0;
      pattern_targets_[0] = PatternCommand{ PatternToken::k_East, 3000 };
      pattern_targets_[1] = PatternCommand{ PatternToken::k_South, 3000 };
      pattern_targets_[2] = PatternCommand{ PatternToken::k_West, 3000 };
      pattern_targets_[3] = PatternCommand{ PatternToken::k_North, 3000 };

      accum_time_pattern_ = 0;
      pattern_step_ = 50;
      speed_ *= 1.25f;
      epsilon_ = kEpsilonFactor * speed_;
      break;
    default:
      break;
    }

    initialized_ = true;
  }
  switch (actual_state_) {
  case FSMStates::k_Working:
    FSM_Working();
    break;
  case FSMStates::k_Chasing:
    FSM_Chasing();
    break;
  case FSMStates::k_Fleeing:
    FSM_Fleeing();
    break;
  case FSMStates::k_Resting:
    FSM_Resting();
    break;
  }

}

void Agent::move(const uint32_t dt)
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

//bool Agent::isPlayerAtSight() const
//{
//  Float2 player_position = GameState::instance().player_->position_;
//  Float2 vec_distance = player_position - position_;
//  float distance = vec_distance.Length();
//  return (player_position - position_).Length() < vision_range_;
//}

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

void Agent::FSM_Working() {

}

void Agent::FSM_Chasing() {

}

void Agent::FSM_Fleeing() {

}

void Agent::FSM_Resting() {

}

void Agent::MOV_Determinist(const uint32_t dt)
{
  if (!positionReached()) return;
  target_reached_ = true;

  const Float2* p = path_.nextPoint();
  setNextPosition(p->x, p->y);
}

void Agent::MOV_Random(const uint32_t dt)
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

void Agent::MOV_Tracking(const uint32_t dt)
{
  accum_time_tracking_ += dt;
  if (accum_time_tracking_ < tracking_retarget_time_ && !positionReached()) return;
  setNextPosition(target_position_.x, target_position_.y);
  accum_time_tracking_ = 0;
}

void Agent::MOV_Pattern(const uint32_t dt)
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

void Agent::MOV_AStar(const uint32_t dt) {
  if (target_reached_)
  {
    if (path_.isLast())
    {
      move_type_ = MovementType::k_MovStop;
    }else
    {
      if (!positionReached()) return;
      target_reached_ = true;

      const Float2* p = path_.nextPoint();
      setNextPosition(p->x, p->y);
    }
  }
}

void Agent::prepareAStar(const Float2& origin, const Float2& dst)
{
  if(path_finder_agent_ && !path_.isReady())
  {
    path_finder_agent_->GeneratePath(&path_, origin, dst);
  }
}

void Agent::startAStar()
{
  move_type_ = MovementType::k_MovAStar;
}
