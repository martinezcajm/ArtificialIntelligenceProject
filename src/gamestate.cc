// gamestate.cc
// Jose Maria Martinez
// Implementation of the gamestate using the singleton pattern.
// Comments for the functions can be found at the header

#include <gamestate.h>

GameState::GameState() {
  quit_game_ = false;
}

GameState& GameState::instance() {
  static GameState* game_instance = new GameState();
  return *game_instance;
}