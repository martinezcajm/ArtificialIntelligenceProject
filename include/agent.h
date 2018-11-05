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

struct Position
{
  float x_;
  float y_;
};

class Agent
{
public:
  Agent();
  Agent(const MovementType mov_type, const float x, const float y);
  void update(const int32_t dt);
  void changeMovementType(const MovementType move_type);
  float x() const;
  float y() const;
private:
  static const int determinist_size_ = 2;
  int determinist_idx_;
  Position determinist_positions_[determinist_size_];
  MovementType move_type_;
  float next_movement_total_;
  float accumulated_movement_;
  float x_;
  float y_;
  float target_x_;
  float target_y_;
  float velocity_x_;
  float velocity_y_;
  const float kSpeed = 0.3f;
  const float kSpeedUp = 2.0f;
  void updateMind(const int32_t dt);
  void updateBody(const int32_t dt);
  void MOV_Determinist();
  void MOV_Random();
  void MOV_Tracking();
  void MOV_Pattern();
  void move(const int32_t dt);
  void calculateVelocity();
  float vectorMagnitude(float x, float y) const;
  bool positionReached() const;
  void setNextPosition(float new_target_x, float new_target_y);
};


#endif
