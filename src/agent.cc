#include "agent.h"
#include <cmath>
#include <gamestate.h>

Agent::Agent()
{
  move_type_ = MovementType::k_MovDeterminist;
  position_ = Float2(0, 0);
  target_position_ = Float2(0, 0);
  velocity_ = Float2(0, 0);

  determinist_idx_ = 0;

  determinist_targets_[0] = Float2(0, 0);
  determinist_targets_[1] = Float2(1000.0f, 0.0f);
}

Agent::Agent(const MovementType mov_type, const float x, const float y) : move_type_(mov_type)
{
  position_ = Float2(x, y);
  target_position_ = Float2(0, 0);
  velocity_ = Float2(0, 0);

  actual_pattern_ = PatternMovement::k_PatRight;

  determinist_idx_ = 0;
  determinist_targets_[0] = Float2(0, 0);
  determinist_targets_[1] = Float2(1000.0f, 0.0f);
}


void Agent::update(const float dt)
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


void Agent::updateBody(const float dt)
{
  //UpdateSensors(); podemos pasarle el delta time para no actualizar en cada frame
  if (isPlayerAtSight()) changeMovementType(MovementType::k_MovTracking);
  switch (move_type_)
  {
  case MovementType::k_MovDeterminist:
    MOV_Determinist();
    break;
  case MovementType::k_MovRandom:
    MOV_Random();
    break;
  case MovementType::k_MovTracking:
    MOV_Tracking();
    break;
  case MovementType::k_MovPattern:
    MOV_Pattern();
    break;
  case MovementType::k_MovStop:
    MOV_Stop();
  default:
    break;
  }
  //TODO Es posible que haya que hacer este calculo de velocidad en los movimientos
  //Ya que el tracking nos puede interesar que mantenga el objetivo (orientacion) durante un tiempo
  calculateVelocity();
  move(dt);
}

void Agent::updateMind(const float dt)
{
  if (move_type_ == MovementType::k_MovTracking) target_position_ = GameState::instance().player_->position_;
}

void Agent::move(const float dt)
{
  //displacement
  Float2 e = velocity_ * dt;
  position_ += e;
  //accumulated_movement_ = accumulated_movement_ + e.Length();

}

bool Agent::positionReached() const
{
  Float2 aux_vector = target_position_ - position_;
  return aux_vector.Length() < kEpsilon;
  //return abs(accumulated_movement_) >= next_movement_total_;
}

bool Agent::isPlayerAtSight() const
{
  Float2 player_position = GameState::instance().player_->position_;
  Float2 vec_distance = player_position - position_;
  float distance = vec_distance.Length();
  return (player_position - position_).Length() < vision_range_;
}

void Agent::setNextPosition(float new_target_x, float new_target_y)
{
  target_position_ = Float2(new_target_x, new_target_y);

  //accumulated_movement_ = 0;
  //Float2 aux_vector = target_ - position_;
  //next_movement_total_ = aux_vector.Length();
}

void Agent::setOrientation(const Float2& destination)
{
  velocity_ = destination - position_;
  velocity_ /= velocity_.Length();
  //Float2 vel = target_position_ - position_;
  //float length = vel.Length();
  //vel /= length;
}

void Agent::calculateVelocity()
{
  //Float2 vel = target_position_ - position_;
  //float length = vel.Length();
  //vel /= length;
  ////quitar ultima linea
  //velocity_ = vel * kSpeed;
  setOrientation(target_position_);
  velocity_ *= kSpeed;
}

void Agent::MOV_Determinist()
{
  if (!positionReached()) return;
  //TODO target_reached_ = true conectar mente con cuerpo

  determinist_idx_ = (determinist_idx_ + 1) % determinist_size_;
  setNextPosition(determinist_targets_[determinist_idx_].x, determinist_targets_[determinist_idx_].y);
}

//TODO cada n unidades de tiempo (n puede ser constante o aleatoria) usar dt para esto
//TODO esto sobretodo es importante para el tracking
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

//TODO cada n unidades de tiempo (n puede ser constante o aleatoria) usar dt para esto
//TODO sobretodo interesante para evitar que la persecucion quede muy robotica (como un misil teledirigido)
void Agent::MOV_Tracking()
{
  if (!positionReached()) return;
  setNextPosition(target_position_.x, target_position_.y);
}

void Agent::MOV_Pattern()
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

void Agent::MOV_Stop()
{
  target_position_ = position_;
}