// path_finder.cc
// Jose Maria Martinez
// Implementation path_finder agent.
//Comments for the functions can be found at the header

#include "path_finder.h"
#include "path.h"
#include "common_def.h"
#include "gamestate.h"

PathFinder::PathFinder()
{
  a_star_ = new AStar();
  id_ = 0;
  actual_state_ = PFAgentState::k_Waiting;
  initialized_ = false;
  requestor_ = -1;
}

PathFinder::~PathFinder()
{
  delete(a_star_);
}


s16 PathFinder::generatePath(/*origin, dest, */ Path* path, Float2 origin, Float2 dst, u32 timeout)
{
  double t = static_cast<double>(timeout) / 1000;  
  return a_star_->generatePath(origin, dst, path, GameState::instance().map_, t);

}

s16 PathFinder::generatePath(/*origin, dest, */ Path* path, Float2 origin, Float2 dst)
{
  return a_star_->generatePath(origin, dst, path, GameState::instance().map_);

}

void PathFinder::update(const u32 dt)
{
  updateMind(dt);
  updateBody(dt);
}

void PathFinder::updateBody(const u32 dt)
{
  
}

void PathFinder::updateMind(const u32 dt)
{
  if(!initialized_)
  {
    num_agents_ = GameState::instance().num_agents_;
    mail_box_ = static_cast<AgentMessage*>(malloc(num_agents_ * sizeof(AgentMessage)));
    for (uint32_t i = 0; i < num_agents_; i++)
    {
      (mail_box_ + i)->type = AgentMessageType::k_Nothing;
      (mail_box_ + i)->position = Float2(0.0f, 0.0f);
    }
    initialized_ = true;
  }

  if(actual_state_ == PFAgentState::k_Waiting)
  {
    for (uint32_t i = 0; i < num_agents_; i++)
    {
      if((mail_box_ + i)->type == AgentMessageType::k_AskForPath)
      {
        requestor_ = i;
        actual_state_ = PFAgentState::k_Calculating;
        origin_ = (mail_box_ + i)->position;
        dst_ = (mail_box_ + i)->dst;
        path_ = (mail_box_ + i)->path;
        (mail_box_ + i)->type = AgentMessageType::k_Nothing;
        (mail_box_ + i)->position = Float2(0.0f, 0.0f);
        break;
      }
    }
  }
  if(actual_state_ == PFAgentState::k_Calculating)
  {
    const s32 status = generatePath(path_, origin_, dst_, dt);
    if(status == kErrorCode_Ok)
    {
      AgentMessage msg;
      msg.type = AgentMessageType::k_PathIsReady;
      msg.position = Float2(0.0f, 0.0f);
      msg.path = nullptr;
      GameState::instance().agents_[requestor_ - 1]->sendMessage(msg, id_);
      actual_state_ = PFAgentState::k_Waiting;
    }else if(status != kErrorCode_Timeout)
    {
      AgentMessage msg;
      msg.type = AgentMessageType::k_PathNotFound;
      msg.position = Float2(0.0f, 0.0f);
      msg.path = nullptr;
      GameState::instance().agents_[requestor_ - 1]->sendMessage(msg, id_);
      actual_state_ = PFAgentState::k_Waiting;
    }
  }
}

void PathFinder::sendMessage(const AgentMessage msg, const u32 id)
{
  *(mail_box_ + id) = msg;
}

