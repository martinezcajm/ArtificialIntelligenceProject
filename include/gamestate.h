// gamestate.h
// Jose Maria Martinez
// Header of the functions of the gamestate class

#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <agent.h>
#include <cstdint>
#include <vector>
#include "map.h"

class PathFinder;

/** @brief GameState entity
*
* Game State of our game based in a Singleton pattern. 
*
*/
class GameState {

private:
  /** @brief Game state constructor
  *
  * privateGame state constructor
  *
  * @return *GameState
  */
  GameState();
  /** @brief GameState copy constructor
  *
  * GameState copy constructor without anything to disable it.
  *
  * @param g the game state we will copy
  *
  * @return *GameState
  */
  GameState(const GameState& g) = delete;
  /** @brief Destroys the GameState
  *
  * Destructor of the GameState
  *
  * @return void
  */
  ~GameState();  
 

public:
  /** @brief Gets the instance of our GameState
  *
  * In charge of creating our GameState singleton in case it does not exist
  * or return its instance if it exists.
  *
  * @return GameState& instance
  */
  static GameState& instance();

  bool quit_game_;

  uint32_t time_step_;

  bool should_game_end_;

  uint32_t num_agents_;

  std::vector<Agent*> agents_;

  PathFinder* pf_agent_;

  uint32_t frequency_;

  Map map_;

};

#endif