// path_finder.h
// Jose Maria Martinez
// Header of the functions of the pathfinder agent class
#ifndef __PATH_FINDER_H__
#define __PATH_FINDER_H__

#include "astar.h"
#include "map.h"

class Path;
class Map;

enum class AgentMessageType
{
  k_Nothing = -1,
  k_AskForPath = 0,
  k_PathIsReady = 1,
  k_PathNotFound = 2
};

struct AgentMessage
{
  Float2 position;
  Float2 dst;
  AgentMessageType type = AgentMessageType::k_Nothing;
  Path* path = nullptr;
};

enum class PFAgentState
{
  k_Waiting = 0,
  k_Calculating = 1,
  k_PADDING = 255
};

/** @brief PathFinder agentt
*
* This class is an agent that can be asked for paths. He utilizes the
* A* algorithm to calculate them
*
*/
class PathFinder
{
public:
  /** @brief Pathfinder Agent constructor
  *
  * Default pathfinder agent constructor
  *
  * @return *PathFinder
  */
  PathFinder::PathFinder();
  /** @brief Destroys the PathFinder
  *
  * Destructor of the pathfinder agent entity
  *
  * @return void
  */
  ~PathFinder();
  /** @brief generates a path from origin to dst within a time
  *
  * Starts the generation of a path.
  * 
  * The possible values it can return are:
  *  kErrorCode_InvalidOrigin if the origin is not a valid position
  *  kErrorCode_InvalidDestination if dst is not a valid position
  *  kErrorCode_Memory in case the function was unable to allocate new memory 
  *  kErrorCode_PathNotCreated 
  *  kErrorCode_PathNotFound if there's not a path that connects origin and dst
  *  kErrorCode_Timeout the A* did not finish to calculate the path. The next time
  *  generate path is called it will continue from the last point.
  *  kErrorCode_Ok the path was correctly calculated
  *
  * @param path Path in which the result will be stored
  * @param origin origin point from which the path will be calculated
  * @param dst Path in which the result will be stored
  * @param timeout Total time to calculate the path
  * @return s16
  */
  s16 generatePath(Path* path, Float2 origin, Float2 dst, u32 timeout);
  /** @brief generates a path from origin to dst
  *
  * Starts the generation of a path.
  *
  * The possible values it can return are:
  *  kErrorCode_InvalidOrigin if the origin is not a valid position
  *  kErrorCode_InvalidDestination if dst is not a valid position
  *  kErrorCode_Memory in case the function was unable to allocate new memory
  *  kErrorCode_PathNotCreated
  *  kErrorCode_PathNotFound if there's not a path that connects origin and dst
  *  kErrorCode_Ok the path was correctly calculated
  *
  * @param path Path in which the result will be stored
  * @param origin origin point from which the path will be calculated
  * @param dst Path in which the result will be stored
  * @return s16
  */
  s16 generatePath(Path* path, Float2 origin, Float2 dst);
  /** @brief Updates the agent
  *
  * Updates the body and mind of the agent based on a delta time
  *
  * @param dt time that has passed in the game world
  * @return void
  */
  void update(const u32 dt);
  /** @brief Updates the mind of the agent
  *
  * Method in charge of the decision making of the agent.
  * Calculates the paths requested by the agents.
  *
  * @param dt time that has passed in the game world
  * @return void
  */
  void updateMind(const u32 dt);
  /** @brief Updates the body of the agent
  *
  * Updates the physic representation of the agent. Deals with
  * the movement and updates the sensors. This agent does not have
  * representation so this function is empty.
  *
  * @param dt time that has passed in the game world
  * @return void
  */
  void updateBody(const u32 dt);
  /** @brief function to receive a message from another agent
  *
  * Function to send a message to the agent. This message will be
  * treated at the updateMind function.
  *
  * @param msg content of the message sent
  * @param id of the one who sent the message
  *
  * @return void
  */
  void sendMessage(const AgentMessage msg, const u32 id);

private:

  u32 id_;

  PFAgentState actual_state_;

  AStar* a_star_;

  bool initialized_;

  u32 num_agents_;

  //Calculated path
  s32 requestor_;

  Float2 origin_;

  Float2 dst_;

  Path* path_;

  //Message variables
  AgentMessage* mail_box_ = nullptr;
  /** @brief Pathfinder Agent copy constructor
  *
  * The pathfinder agent cannot be copied
  *
  * @return *PathFinder
  */
  PathFinder(const PathFinder& pf)  = delete;
  /** @brief Pathfinder Agent copy operation
  *
  * The pathfinder agent cannot be copied
  *
  * @return *PathFinder
  */
  PathFinder operator=(const PathFinder& pf) = delete;
};

#endif