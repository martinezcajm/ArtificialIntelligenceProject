#include <gamestate.h>

GameState::GameState() {
  quit_game_ = false;
  time_step_ = 16;
}

GameState& GameState::instance() {
  static GameState* game_instance = new GameState();
  return *game_instance;
}