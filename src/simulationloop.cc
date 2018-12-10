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

//typedef enum
//{
//  kNothing = 0,
//  kExit = 1
//} Command;
//
//Command actual_command = kNothing;
//bool quit_game_ = false;

GameState& g_game_state = GameState::instance();

/** @brief Init
*
*Initializes the game state and the window
*
* @return void
*/
void Init() {

  //game_state_.actual_command_ = kNothing;
  g_game_state.quit_game_ = false;
  g_game_state.should_game_end_ = false;
  g_game_state.frequency_ = 60;
  //Maximum time for a frequency of 60 frames per second (1/60)
  //g_game_state.time_step_ = 16;

  //Maximum time in milliseconds for our frequency (1/frequency)
  g_game_state.time_step_ = (1.0/g_game_state.frequency_)*1000;

  ESAT::WindowInit(1280, 720);

  g_game_state.agent_spr_ = ESAT::SpriteFromFile("../data/agent.png");

  g_game_state.agents_.emplace_back(new Agent(AgentType::k_Scout, 1000, 500));
  g_game_state.agents_.emplace_back(new Agent(AgentType::k_Patrol, 20, 250));
  g_game_state.agents_.emplace_back(new Agent(AgentType::k_Mindless, 0, 0));
  g_game_state.agents_.emplace_back(new Agent(AgentType::k_Chaser, 1100, 650));

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

  for (Agent* agent: g_game_state.agents_)
  {
    ESAT::DrawSprite(g_game_state.agent_spr_, agent->x(), agent->y());
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
    g_game_state.should_game_end_ = true;
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
  if (!ESAT::WindowIsOpened()) g_game_state.quit_game_ = true;
  if (g_game_state.should_game_end_) g_game_state.quit_game_ = true;
  for (Agent* agent : g_game_state.agents_)
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
  uint32_t idx = g_game_state.agents_.size()-1;
  while(!g_game_state.agents_.empty())
  {
    Agent* a = g_game_state.agents_[idx];
    delete a;
    g_game_state.agents_.pop_back();
    idx--;
  }
  ESAT::SpriteRelease(g_game_state.agent_spr_);

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
  while (!g_game_state.quit_game_) {

    InputService();
    double accum_time = Time() - current_time;
    while (accum_time >= g_game_state.time_step_)
    {
      Update(g_game_state.time_step_);
      current_time += g_game_state.time_step_;
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