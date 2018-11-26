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
  Agent(const MovementType mov_type, const float x, const float y, const float speed);
  void update(const uint32_t dt);
  void changeMovementType(const MovementType move_type);
  float x() const;
  float y() const;
  bool is_tracking() const;
  void set_is_tracking(bool tracks);
private:
  uint32_t id_ = 0;
  float pattern_step_ = 50;
  const float kEpsilon = 10;
  static const int determinist_size_ = 2;
  int determinist_idx_;
  Float2 determinist_targets_[determinist_size_];
  MovementType move_type_;
  PatternMovement actual_pattern_;
  bool is_tracking_;
  bool target_reached_;

  
  //Agent* target;
  Float2 position_;
  Float2 target_position_;
  Float2 velocity_;

  float vision_range_ = 200;
  //speed as m/s is pixels/s
  float speed_ = 100.0f;
  const float kSpeedUp = 2.0f;

  void init(const float x, const float y);

  void updateMind(const uint32_t dt);
  void updateBody(const uint32_t dt);
  void MOV_Determinist(const uint32_t dt);
  void MOV_Random(const uint32_t dt);
  void MOV_Tracking(const uint32_t dt);
  void MOV_Pattern(const uint32_t dt);
  void MOV_Stop(const uint32_t dt);
  void move(const uint32_t dt);
  void setOrientation(const Float2& destination);
  void calculateVelocity();
  bool positionReached() const;
  bool isPlayerAtSight() const;
  void setNextPosition(float new_target_x, float new_target_y);
};


#endif
