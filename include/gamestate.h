#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <agent.h>
#include <ESAT/sprite.h>
#include <cstdint>
//#include <float2.h>

class GameState {

private:
  GameState();
  GameState(const GameState& g) {};
  ~GameState();

 

public:
  static GameState& instance();

  //Float2 player_position;

  bool quit_game_;

  float time_step_;

  bool should_game_end_;

  ESAT::SpriteHandle agent_spr_;

  ESAT::SpriteHandle player_spr_;

  Agent *test_agent_;

  Agent *player_;

  int32_t frequency_;

};

#endif