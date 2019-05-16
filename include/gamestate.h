// gamestate.h
// Jose Maria Martinez
// Header of the functions of the gamestate class
#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <agent.h>
#include <ESAT/sprite.h>
#include <cstdint>
#include <vector>
#include "map.h"
//#include "path_finder.h"

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
  * @return *GameState
  * @param g the game state we will copy
  */
  GameState(const GameState& g) {};
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

  //Float2 player_position;

  bool quit_game_;

  uint32_t time_step_;

  bool should_game_end_;

  //ESAT::SpriteHandle agent_spr_;

  std::vector<Agent*> agents_;

  PathFinder* pf_agent;

  uint32_t frequency_;

  Map map_;

};

#endif