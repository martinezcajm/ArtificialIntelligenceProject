#ifndef __AGENT_H__
#define __AGENT_H__
#include <cstdint>

typedef enum
{
  kMovDeterminist = 0,
  kMovRandom = 1,
  kMovTracking = 2,
  kMovPattern = 3,
  PADDING = 255
} MovementType;

class Agent
{
public:
  Agent();
  void Update(const int32_t dt);
  void ChangeMovementType(const MovementType move_type);
private:
  MovementType move_type_;
  const float kSpeed = 5.0f;
  const float kSpeedUp = 2.0f;
  void UpdateMind(const int32_t dt);
  void UpdateBody(const int32_t dt);
  void MOV_Determinist();
  void MOV_Random();
  void MOV_Tracking();
  void MOV_Pattern();
  void Move(const int32_t dt);
};


#endif
