#ifndef __AGENT_H__
#define __AGENT_H__
#include <cstdint>
#include <float2.h>

enum class MovementType
{
  k_MovDeterminist = 0,
  k_MovRandom = 1,
  k_MovTracking = 2,
  k_MovPattern = 3,
  k_MovStop = 4,
  k_PADDING = 255
};

enum class PatternMovement
{
  k_PatRight = 0,
  k_PatLeft = 1,
  k_PatDown = 2,
  k_PatUp = 3,
  k_PADDING = 255
};

class Agent
{
public:
  Agent();
  Agent(const MovementType mov_type, const float x, const float y);
  void update(const float dt);
  void changeMovementType(const MovementType move_type);
  float x() const;
  float y() const;
private:
  float pattern_step_ = 50;
  const float kEpsilon = 10;
  static const int determinist_size_ = 2;
  int determinist_idx_;
  Float2 determinist_targets_[determinist_size_];
  MovementType move_type_;
  PatternMovement actual_pattern_;
  //float next_movement_total_;
  //float accumulated_movement_;
  
  //Agent* target;
  Float2 position_;
  Float2 target_position_;
  Float2 velocity_;

  float vision_range_ = 200;
  //speed as m/s
  const float kSpeed = 0.5f;
  const float kSpeedUp = 2.0f;

  void updateMind(const float dt);
  void updateBody(const float dt);
  void MOV_Determinist();
  void MOV_Random();
  void MOV_Tracking();
  void MOV_Pattern();
  void MOV_Stop();
  void move(const float dt);
  void setOrientation(const Float2& destination);
  void calculateVelocity();
  bool positionReached() const;
  bool isPlayerAtSight() const;
  void setNextPosition(float new_target_x, float new_target_y);
};


#endif
