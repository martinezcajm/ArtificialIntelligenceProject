// agent.cc
// Jose Maria Martinez
// Implementation of the agent. Movement functionality and decision making
// functions.
//Comments for the functions can be found at the header

#include <cmath>
#include "agent.h"
#include "gamestate.h"

static uint32_t total_agents = 0;

Agent::Agent() : type_agent_(AgentType::k_Mindless)
{
  init(0, 0);
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
    MOV_Stop(dt);
  default:
    break;
  }
  calculateVelocity();
  move(dt);
}

void Agent::updateMind(const uint32_t dt)
{
  if (!initialized_)
  {
    const float generic_speed = 5.0f;
    speed_ = generic_speed;
    switch (type_agent_)
    {
    case AgentType::k_Mindless:
      move_type_ = MovementType::k_MovDeterminist;

      determinist_idx_ = 0;
      determinist_targets_[0] = Float2(0, 0);
      determinist_targets_[1] = Float2(1000.0f, 0.0f);

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
    default:
      break;
    }

    initialized_ = true;
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
  velocity_ /= velocity_.Length();
}

void Agent::calculateVelocity()
{
  setOrientation(target_position_);
  velocity_ *= speed_;
}

void Agent::MOV_Determinist(const uint32_t dt)
{
  if (!positionReached()) return;
  //TODO target_reached_ = true conectar mente con cuerpo

  determinist_idx_ = (determinist_idx_ + 1) % determinist_size_;
  setNextPosition(determinist_targets_[determinist_idx_].x, determinist_targets_[determinist_idx_].y);
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

void Agent::MOV_Stop(const uint32_t dt)
{
  target_position_ = position_;
}