// simulationloop.cc
// Jose Maria Martinez
// Implementation of the game loop. Simulates the time passed in the game world.
// Is in charge of translating the input, updating our agents and drawing the scene.

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

/** @brief Init
*
*Initializes the game state and the window
*
* @return void
*/
void Init() {

  //game_state_.actual_command_ = kNothing;
  game_state.quit_game_ = false;
  game_state.should_game_end_ = false;
  game_state.frequency_ = 60;
  //Maximum time for a frequency of 60 frames per second (1/60)
  //game_state.time_step_ = 16;

  //Maximum time in milliseconds for our frequency (1/frequency)
  game_state.time_step_ = (1.0/game_state.frequency_)*1000;

  ESAT::WindowInit(1280, 720);

  game_state.agent_spr_ = ESAT::SpriteFromFile("../data/agent.png");

  game_state.agents_.emplace_back(new Agent(MovementType::k_MovRandom, 1000, 500, 100));
  game_state.agents_.emplace_back(new Agent(MovementType::k_MovPattern, 0, 500, 100));
  game_state.agents_.emplace_back(new Agent(MovementType::k_MovDeterminist, 0, 0, 100));
  game_state.agents_.emplace_back(new Agent(MovementType::k_MovTracking, 1100, 650, 100));

}

/** @brief Draw
*
* Function in charge of all the render
*
* @return void
*/
void Draw() {
  ESAT::DrawBegin();
  ESAT::DrawClear(0, 0, 0);
  ESAT::DrawSetFillColor(255, 0, 0);
  ESAT::DrawSetStrokeColor(0, 0, 255);

  for (Agent* agent: game_state.agents_)
  {
    ESAT::DrawSprite(game_state.agent_spr_, agent->x(), agent->y());
  }

  ESAT::DrawEnd();
  ESAT::WindowFrame();
}

/** @brief InputService
*
* Translates the input received so it can be used by game
*
* @return void
*/
void InputService()
{
  if (ESAT::IsSpecialKeyDown(ESAT::kSpecialKey_Escape))
  {
    //game_state_.actual_command_ = kExit;
    game_state.should_game_end_ = true;
  }
}

/** @brief Update
*
* updates all the logic of our game.
* 
* @return void
* @param dt time that has passed in the game world
*/
void Update(uint32_t dt)
{
  if (!ESAT::WindowIsOpened()) game_state.quit_game_ = true;
  if (game_state.should_game_end_) game_state.quit_game_ = true;
  for (Agent* agent : game_state.agents_)
  {
    agent->update(dt);
  }
}

/** @brief Deinit
*
* releases all the memory allocated at init by the game loop 
* to avoid  memory leaks.
*
* @return *GameState
*/
void Deinit()
{
  uint32_t idx = game_state.agents_.size()-1;
  while(!game_state.agents_.empty())
  {
    Agent* a = game_state.agents_[idx];
    delete a;
    game_state.agents_.pop_back();
    idx--;
  }
  ESAT::SpriteRelease(game_state.agent_spr_);

}

/** @brief main
*
* main function of the game.
*
* @return int status of the main function
*/
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
    // fps counter used to test the simulation loop with an extress
    // in the logic part and the render one

    /*frames++;
    double loop_actual_time = Time();
    if (loop_actual_time > loop_last_time + 1000) {
      printf("\n %i FPS: ", frames);
      frames = 0;
      loop_last_time = loop_actual_time;
    }*/
  }
  Deinit();
  return 0;
}