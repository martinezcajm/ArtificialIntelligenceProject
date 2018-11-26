#include <gamestate.h>

GameState::GameState() {
  quit_game_ = false;
}

GameState& GameState::instance() {
  static GameState* game_instance = new GameState();
  return *game_instance;
}