#include "agent.h"
//#include <algorithm>
//#include <math.h>
#include <cmath>
//#include <numeric>
//#define RAND_MAX 1

Agent::Agent()
{
  move_type_ = kMovRandom;
  x_ = 0;
  y_ = 100;
  target_x_ = 0;
  target_y_ = 0;
  velocity_x_ = 0;
  velocity_y_ = 0;

  determinist_idx_ = 0;

  determinist_positions_[0].x_ = 0.0f;
  determinist_positions_[0].y_ = 0.0f;
  determinist_positions_[1].x_ = 1000.0f;
  determinist_positions_[1].y_ = 0.0f;

  next_movement_total_ = 0;
  accumulated_movement_ = 0;
}

Agent::Agent(const MovementType mov_type, const float x, const float y) : move_type_(mov_type), x_(x), y_(y)
{
  target_x_ = 0;
  target_y_ = 0;
  velocity_x_ = 0;
  velocity_y_ = 0;

  determinist_idx_ = 0;

  determinist_positions_[0].x_ = 0.0f;
  determinist_positions_[0].y_ = 0.0f;
  determinist_positions_[1].x_ = 1000.0f;
  determinist_positions_[1].y_ = 0.0f;
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
  return x_;
}

float Agent::y() const
{
  return y_;
}


void Agent::updateBody(const int32_t dt)
{
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
  const float ex = velocity_x_ * dt;
  const float ey = velocity_y_ *dt;
  x_ = x_ + ex;
  y_ = y_ + ey;
  accumulated_movement_ = accumulated_movement_ + abs(ex) + abs(ey);

}

bool Agent::positionReached() const
{
  return abs(accumulated_movement_) >= next_movement_total_;
}

void Agent::setNextPosition(float new_target_x, float new_target_y)
{
  target_x_ = new_target_x;
  target_y_ = new_target_y;

  accumulated_movement_ = 0;
  next_movement_total_ = vectorMagnitude(target_x_ - x_, target_y_ - y_);
}

void Agent::calculateVelocity()
{
  float vel_x = target_x_ - x_;
  float vel_y = target_y_ - y_;
  float vector_magnitude = vectorMagnitude(vel_x, vel_y);
  vel_x = vel_x / vector_magnitude;
  vel_y = vel_y / vector_magnitude;
  velocity_x_ = vel_x*kSpeed;
  velocity_y_ = vel_y*kSpeed;
}

void Agent::MOV_Determinist()
{
  if (!positionReached()) return;

  determinist_idx_ = (determinist_idx_ + 1) % determinist_size_;
  setNextPosition(determinist_positions_[determinist_idx_].x_, determinist_positions_[determinist_idx_].y_);
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
  
}

void Agent::MOV_Pattern()
{

}

float Agent::vectorMagnitude(float x, float y) const
{
  return sqrtf((x*x) + (y*y));
}
