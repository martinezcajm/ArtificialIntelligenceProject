#ifndef __AGENT_H__
#define __AGENT_H__
#include <cstdint>

class Agent
{
public:
  Agent();
  void Update(int32_t dt);
private:
  const float kSpeed = 5.0f;
  const float kSpeedUp = 2.0f;
  void UpdateMind(int32_t dt);
  void UpdateBody(int32_t dt);
};


#endif
