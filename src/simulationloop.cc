#include <ESAT/window.h>
#include <ESAT/input.h>
#include <ESAT/time.h>
#include <ESAT/draw.h>
#include <ESAT/sprite.h>
#include <cstdint>
#include <cmath>
#include <cstdio>
#include <agent.h>
#include <gamestate.h>

typedef enum
{
  kNothing = 0,
  kExit = 1
} Command;
//
//Command actual_command = kNothing;
//bool quit_game_ = false;

GameState& game_state = GameState::instance();

void Init() {

  //game_state_.actual_command_ = kNothing;
  game_state.quit_game_ = false;
  game_state.should_game_end_ = false;
  game_state.frequency_ = 60;
  //Maximum time for a frequency of 60 frames per second (1/60)
  //game_state.time_step_ = 16;

  //Maximum time in milliseconds for our frequency (1/frequency)
  game_state.time_step_ = (1.0f/game_state.frequency_);

  ESAT::WindowInit(1280, 720);

  game_state.agent_spr_ = ESAT::SpriteFromFile("../data/agent.png");
  game_state.player_spr_ = ESAT::SpriteFromFile("../data/player.png");
  game_state.test_agent_ = new Agent(MovementType::k_MovPattern, 0, 0);
  game_state.player_ = new Agent(MovementType::k_MovStop, 500, 500);
}

void Draw() {
  ESAT::DrawBegin();
  ESAT::DrawClear(0, 0, 0);
  ESAT::DrawSetFillColor(255, 0, 0);
  ESAT::DrawSetStrokeColor(0, 0, 255);

  ESAT::DrawSprite(game_state.agent_spr_, game_state.test_agent_->x(),
    game_state.test_agent_->y());
  ESAT::DrawSprite(game_state.player_spr_, game_state.player_->x(),
    game_state.player_->y());

  ESAT::DrawEnd();
  ESAT::WindowFrame();
}

void InputService()
{
  if (ESAT::IsSpecialKeyDown(ESAT::kSpecialKey_Escape))
  {
    //game_state_.actual_command_ = kExit;
    game_state.should_game_end_ = true;
  }
}

void Update(float dt)
{
  if (!ESAT::WindowIsOpened()) game_state.quit_game_ = true;
  if (game_state.should_game_end_) game_state.quit_game_ = true;
  game_state.test_agent_->update(dt);
}

int ESAT::main(int argc, char **argv) {


  Init();
  unsigned int frames = 0;
  double current_time = Time();
  double loop_last_time = Time();
  // main loop
  while (!game_state.quit_game_) {

    InputService();
    double accum_time = Time() - current_time;
    while (accum_time >= game_state.time_step_)
    {
      Update(game_state.time_step_);
      current_time += game_state.time_step_;
      accum_time = Time() - current_time;
    }
    Draw();
    frames++;
    double loop_actual_time = Time();
    if (loop_actual_time > loop_last_time + 1000) {
      //printf("\n %i FPS: ", frames);
      frames = 0;
      loop_last_time = loop_actual_time;
    }
  }
  return 0;
}