#include "agent_implementation.h"

static uint32_t total_agents = 0;

AgentImplementation::AgentImplementation()
{
  id_ = total_agents;
  total_agents++;
}
