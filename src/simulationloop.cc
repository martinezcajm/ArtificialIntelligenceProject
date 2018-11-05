#include <ESAT/window.h>
#include <ESAT/input.h>
#include <ESAT/time.h>
#include <ESAT/draw.h>
#include <ESAT/sprite.h>
#include <cstdint>
#include <cmath>
#include <stdio.h>
#include <agent.h>

typedef enum
{
  kNothing = 0,
  kExit = 1
} Command;
//
//Command actual_command = kNothing;
//bool quit_game_ = false;

struct GameState{
  int32_t frequency_;
	int32_t time_step_;
	Command actual_command_;
	bool quit_game_;
  bool should_game_end_;
  ESAT::SpriteHandle agent_spr_;
  Agent *test_agent_;
}game_state_;


void Init() {
	
	game_state_.actual_command_ = kNothing;
	game_state_.quit_game_ = false;
  game_state_.frequency_ = 60;
  //Maximum time for a frequency of 60 frames per second (1/60)
  game_state_.time_step_ = 16;

	ESAT::WindowInit(1280, 720);

  game_state_.agent_spr_ = ESAT::SpriteFromFile("../data/agent.png");
  game_state_.test_agent_ = new Agent();
}

void Draw() {
  ESAT::DrawBegin();
  ESAT::DrawClear(0, 0, 0);
  ESAT::DrawSetFillColor(255, 0, 0);
  ESAT::DrawSetStrokeColor(0, 0, 255);
  //static float advance = 0;
  for (int i = 0; i < 1; i++) {
      for (int i = 0; i < 1; i++) {
          //ESAT::DrawLine(advance, advance, advance+50, advance+50);
          ESAT::DrawSprite(game_state_.agent_spr_, game_state_.test_agent_->x(),
           game_state_.test_agent_->y());
      }
  }
  /*advance += 5;
  if (advance > 720) advance = 0;*/
  ESAT::DrawEnd();
	ESAT::WindowFrame();
}

void InputService()
{
	if(ESAT::IsSpecialKeyDown(ESAT::kSpecialKey_Escape))
	{
		//game_state_.actual_command_ = kExit;
		game_state_.should_game_end_ = true;
	}
}

void Update(int32_t dt)
{
  if (!ESAT::WindowIsOpened()) game_state_.quit_game_ = true;
  if (game_state_.should_game_end_) game_state_.quit_game_ = true;
  game_state_.test_agent_->update(dt);
  //for (int i = 0; i < 500; i++) {
	 // for (int i = 0; i < 500; i++) {
		//  sqrt(16);
  //    
	 // }
  //}
}

int ESAT::main(int argc, char **argv) {
  
  
  Init();
  unsigned int frames = 0;
  double current_time = Time();
  double loop_last_time = Time();
	// main loop
  while (!game_state_.quit_game_) {
    
		InputService();
		double accum_time = Time() - current_time;
		while (accum_time >= game_state_.time_step_)
		{
		  Update(game_state_.time_step_);
		  current_time += game_state_.time_step_;
		  //accum_time = Time() - current_time;
      accum_time = Time() - current_time;
		}
		Draw();
    frames++;  
    double loop_actual_time = Time();
    if (loop_actual_time > loop_last_time + 1000) {        
        printf("\n %i FPS: ", frames);
        frames = 0;
        loop_last_time = loop_actual_time;
    }
	}
	return 0;
}