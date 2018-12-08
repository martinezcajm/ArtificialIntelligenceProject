// agent.cc
// Jose Maria Martinez
// Implementation of the agent. Movement functionality and decision making
// functions.
//Comments for the functions can be found at the header

#include "agent.h"
#include <cmath>
#include <gamestate.h>

static uint32_t total_agents = 0;

Agent::Agent() : move_type_(MovementType::k_MovDeterminist)
{
  init(0, 0);
}

Agent::Agent(const MovementType mov_type, const float x, const float y, const float speed) : move_type_(mov_type), speed_(speed)
{
  init(x, y);
}

Agent::~Agent()
{
  
}


void Agent::init(const float x, const float y)
{
  epsilon_ = kEpsilon * speed_;
  position_ = Float2(x, y);
  target_position_ = position_;
  velocity_ = Float2(0, 0);

  actual_pattern_ = PatternMovement::k_PatRight;

  determinist_idx_ = 0;
  determinist_targets_[0] = Float2(0, 0);
  determinist_targets_[1] = Float2(1000.0f, 0.0f);


  id_ = total_agents;
  total_agents++;
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
  static bool initialized = false;
  if (initialized) return;
  const float generic_speed = 50.0f;
  switch (move_type_)
  {
  case MovementType::k_MovDeterminist:
    speed_ = generic_speed * 0.75f;
    epsilon_ = kEpsilon * speed_;
    break;
  case MovementType::k_MovRandom:
    speed_ = generic_speed * 1.0f;
    epsilon_ = kEpsilon * speed_;
    break;
  case MovementType::k_MovTracking:
    speed_ = generic_speed * 1.25f;
    epsilon_ = kEpsilon * speed_;
    target_position_ = Float2(640,360);
    break;
  case MovementType::k_MovPattern:
    speed_ = generic_speed * 1.50f;
    epsilon_ = kEpsilon * speed_;
    break;
  default:
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
  const uint32_t change_position_time = 5000; //5 s
  static uint32_t accum_time = 5000;

  accum_time += dt;

  if (accum_time < change_position_time && !positionReached()) return;
  //if (!positionReached()) return;
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

  accum_time = 0;
}

void Agent::MOV_Tracking(const uint32_t dt)
{
  const uint32_t retarget_time = 3000; //3 s
  static uint32_t accum_time = 0;
  accum_time += dt;
  if (accum_time < retarget_time && !positionReached()) return;
  //target_reached_ = true;
  setNextPosition(target_position_.x, target_position_.y);
  accum_time = 0;
}

void Agent::MOV_Pattern(const uint32_t dt)
{
  if (!positionReached())return;
  static int cycles = 0;
  switch (actual_pattern_)
  {
  case PatternMovement::k_PatRight:
    target_position_ = Float2(position_.x + pattern_step_, position_.y);
    if (cycles == 2)
    {
      cycles = 0;
      actual_pattern_ = PatternMovement::k_PatDown;
    }
    break;
  case PatternMovement::k_PatLeft:
    target_position_ = Float2(position_.x - pattern_step_, position_.y);
    if (cycles == 2)
    {
      cycles = 0;
      actual_pattern_ = PatternMovement::k_PatUp;
    }
    break;
  case PatternMovement::k_PatUp:
    target_position_ = Float2(position_.x, position_.y - pattern_step_);
    if (cycles == 2)
    {
      cycles = 0;
      actual_pattern_ = PatternMovement::k_PatRight;
    }
    break;
  case PatternMovement::k_PatDown:
    target_position_ = Float2(position_.x, position_.y + pattern_step_);
    if (cycles == 2)
    {
      cycles = 0;
      actual_pattern_ = PatternMovement::k_PatLeft;
    }
    break;
  default:
    break;
  }
  cycles++;
}

void Agent::MOV_Stop(const uint32_t dt)
{
  target_position_ = position_;
}