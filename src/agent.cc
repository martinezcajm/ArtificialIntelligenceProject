#include "agent.h"
#include <cmath>

Agent::Agent()
{
  move_type_ = kMovDeterminist;
  position_ = Float2(0, 0);
  target_ = Float2(0, 0);
  velocity_ = Float2(0, 0);

  determinist_idx_ = 0;

  determinist_targets_[0] = Float2(0, 0);
  determinist_targets_[1] = Float2(1000.0f, 0.0f);


  //next_movement_total_ = 0;
  //accumulated_movement_ = 0;
}

Agent::Agent(const MovementType mov_type, const float x, const float y) : move_type_(mov_type)
{
  position_ = Float2(x, y);
  target_ = Float2(0, 0);
  velocity_ = Float2(0, 0);

  player_position_ = Float2(500, 500);
  actual_pattern_ = kPatRight;

  determinist_idx_ = 0;
  determinist_targets_[0] = Float2(0, 0);
  determinist_targets_[1] = Float2(1000.0f, 0.0f);
}


void Agent::update(const int32_t dt)
{
  updateMind(dt);
  updateBody(dt);
}

void Agent::changeMovementType(const MovementType move_type)
{
  move_type_ = move_type;
}

float Agent::x() const
{
  return position_.x;
}

float Agent::y() const
{
  return position_.y;
}


void Agent::updateBody(const int32_t dt)
{
  if (isPlayerAtSight()) changeMovementType(kMovTracking);
  switch(move_type_)
  {
  case MovementType::kMovDeterminist:
    MOV_Determinist();
    break;
  case MovementType::kMovRandom:
    MOV_Random();
    break;
  case MovementType::kMovTracking:
    MOV_Tracking();
    break;
  case MovementType::kMovPattern:
    MOV_Pattern();
    break;
  case MovementType::kMovStatic:
    target_ = position_;
  default:
    break;
  }
  calculateVelocity();
  move(dt);
}

void Agent::updateMind(const int32_t dt)
{
  
}

void Agent::move(const int32_t dt)
{
  //displacement
  Float2 e = velocity_ * dt;
  position_ += e;
  //accumulated_movement_ = accumulated_movement_ + e.Length();

}

bool Agent::positionReached() const
{
  Float2 aux_vector = target_ - position_;
  return aux_vector.Length() < kEpsilon;
  //return abs(accumulated_movement_) >= next_movement_total_;
}

bool Agent::isPlayerAtSight() const
{
  Float2 vec_distance = player_position_ - position_;
  float distance = vec_distance.Length();
  return (player_position_ - position_).Length() < vision_range_;
}

void Agent::setNextPosition(float new_target_x, float new_target_y)
{
  target_ = Float2(new_target_x, new_target_y);

  //accumulated_movement_ = 0;
  //Float2 aux_vector = target_ - position_;
  //next_movement_total_ = aux_vector.Length();
}

void Agent::calculateVelocity()
{
  Float2 vel = target_ - position_;
  float length = vel.Length();
  vel /= length;
  velocity_ = vel * kSpeed;
}

void Agent::MOV_Determinist()
{
  if (!positionReached()) return;

  determinist_idx_ = (determinist_idx_ + 1) % determinist_size_;
  setNextPosition(determinist_targets_[determinist_idx_].x, determinist_targets_[determinist_idx_].y);
}

void Agent::MOV_Random()
{
  if (!positionReached()) return;
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
}

void Agent::MOV_Tracking()
{
  if(isPlayerAtSight())
  {
    target_ = player_position_;
  }
}

void Agent::MOV_Pattern()
{
  if (!positionReached())return;
  static int cycles = 0;
  switch(actual_pattern_)
  {
  case kPatRight:
    target_ = Float2(position_.x + pattern_step_, position_.y);
    if(cycles == 2)
    {
      cycles = 0;
      actual_pattern_ = kPatDown;
    }
    break;
  case kPatLeft:
    target_ = Float2(position_.x - pattern_step_, position_.y);
    if (cycles == 2)
    {
      cycles = 0;
      actual_pattern_ = kPatUp;
    }
    break;
  case kPatUp:
    target_ = Float2(position_.x, position_.y - pattern_step_);
    if (cycles == 2)
    {
      cycles = 0;
      actual_pattern_ = kPatRight;
    }
    break;
  case kPatDown:
    target_ = Float2(position_.x, position_.y + pattern_step_);
    if (cycles == 2)
    {
      cycles = 0;
      actual_pattern_ = kPatLeft;
    }
    break;
  default:
    break;
  }
  cycles++;
}

float Agent::vectorMagnitude(float x, float y) const
{
  return sqrtf((x*x) + (y*y));
}
