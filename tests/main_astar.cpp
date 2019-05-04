// main_astar.cc
// Jose Maria Martinez
// Implementation of the A* algorithm

#include "ESAT/window.h"
#include "ESAT/time.h"
#include "gamestate.h"
#include "ESAT/input.h"
#include "ESAT/draw.h"
#include "path.h"
#include "path_finder.h"

GameState& g_game_state = GameState::instance();
bool g_mouse_pressed_ = false;
bool s_pressed_ = false;
Path* p;

/** @brief Init
*
*Initializes the game state and the window
*
* @return void
*/
void Init() {
  g_mouse_pressed_ = false;
  //game_state_.actual_command_ = kNothing;
  g_game_state.quit_game_ = false;
  g_game_state.should_game_end_ = false;
  g_game_state.frequency_ = 60;
  //Maximum time for a frequency of 60 frames per second (1/60)
  //g_game_state.time_step_ = 16;

  //Maximum time in milliseconds for our frequency (1/frequency)
  g_game_state.time_step_ = static_cast<uint32_t>((1.0 / g_game_state.frequency_) * 1000);

  ESAT::WindowInit(960, 704);
  ESAT::WindowSetMouseVisibility(true);

  g_game_state.agent_spr_ = ESAT::SpriteFromFile("../../../data/gfx/agents/allied_soldier.bmp");
  g_game_state.screen_spr_ = ESAT::SpriteFromFile("../../../data/gfx/maps/map_03_960x704_layout ABGS.png");


  

  //Path* p = new Path();
  //Path finder agent
  g_game_state.pf_agent = new PathFinder();
  g_game_state.pf_agent->LoadMap("../../../data/gfx/maps/map_03_120x88_cost.png", 960, 704);
  //g_game_state.pf_agent->GeneratePath(p);

  //g_game_state.agents_.emplace_back(new Agent(AgentType::k_Hero, 416, 32));
  g_game_state.agents_.emplace_back(new Agent(AgentType::k_Hero, 0, 0));
  
  delete p;
  int a = 3;
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
  if (ESAT::MouseButtonDown(0)) g_mouse_pressed_ = true;
  if (ESAT::IsSpecialKeyDown(ESAT::kSpecialKey_F1)) s_pressed_ = true;
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
  if (g_mouse_pressed_) {
    g_mouse_pressed_ = false;
    printf("mouse pressed at {%f,%f} \n", ESAT::MousePositionX(), ESAT::MousePositionY());
  }
  if(s_pressed_)
  {
    s_pressed_ = false;
    g_game_state.agents_[0]->startAStar();
  }
  for (Agent* agent : g_game_state.agents_)
  {
    agent->update(dt);
  }
  
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

  ESAT::DrawSprite(g_game_state.screen_spr_, 0, 0);

  //ESAT::DrawSprite("", 0, 0);

  for (Agent* agent : g_game_state.agents_)
  {
    ESAT::DrawSprite(g_game_state.agent_spr_, agent->x(), agent->y());
  }

  ESAT::DrawEnd();
  ESAT::WindowFrame();
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
  uint32_t idx = g_game_state.agents_.size() - 1;
  while (!g_game_state.agents_.empty())
  {
    Agent* a = g_game_state.agents_[idx];
    delete a;
    g_game_state.agents_.pop_back();
    idx--;
  }
  ESAT::SpriteRelease(g_game_state.agent_spr_);
  ESAT::SpriteRelease(g_game_state.screen_spr_);
  delete(g_game_state.pf_agent);
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