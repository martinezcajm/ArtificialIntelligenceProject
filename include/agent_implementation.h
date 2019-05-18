#ifndef __AGENT_IMPLEMENTATION__
#define __AGENT_IMPLEMENTATION__

#include "platform_types.h"

class AgentImplementation
{
public:
  /** @brief Entity constructor
  *
  * Base constructor of entity
  *
  * @return *Entity
  */
  AgentImplementation();
  /** @brief Destroys the Entity
  *
  * Destructor of the entity
  *
  * @return void
  */
  virtual ~AgentImplementation() = 0{};
private:
  uint32_t id_ = 0;
};

#endif