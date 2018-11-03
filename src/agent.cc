#include "agent.h"

Agent::Agent()
{
  move_type_ = kMovDeterminist;
}

void Agent::Update(const int32_t dt)
{
  UpdateMind(dt);
  UpdateBody(dt);
}

void Agent::ChangeMovementType(const MovementType move_type)
{
  move_type_ = move_type;
}


void Agent::UpdateBody(const int32_t dt)
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
  }
  Move(dt);
}

void Agent::UpdateMind(const int32_t dt)
{
  
}

void Agent::Move(const int32_t dt)
{
  
}

void Agent::MOV_Determinist()
{
  
}

void Agent::MOV_Random()
{
  
}

void Agent::MOV_Tracking()
{
  
}

void Agent::MOV_Pattern()
{

}