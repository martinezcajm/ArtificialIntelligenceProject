// astar.cc
// Jose Maria Martinez
// Implementation of the A* algorithm.
//Comments for the functions can be found at the header

#include "astar.h"
#include "path.h"
#include "map.h"
#include <stack>
#include <algorithm>
#include <ESAT/time.h>

static const AgentDirection g_directions[8] = { AgentDirection::k_Northwest, AgentDirection::k_North, AgentDirection::k_Northeast,
                                              AgentDirection::k_West, AgentDirection::k_East,
                                              AgentDirection::k_Southwest, AgentDirection::k_South, AgentDirection::k_Southeast };

AStarNode::AStarNode(Float2 pos, AStarNode* p, s32 step)
{
  position = pos;
  parent = p;
  if(parent)
  {
    g = parent->g + step;
    /*h = 0;
    f = g + h;*/
  }else
  {
    g = 0;
    /*h = 0;
    f = 0;*/
  }
  
}

AStarNode::~AStarNode()
{
  
}

bool AStarNode::hasSameState(const AStarNode& node)
{
  return (position.x == node.position.x &&
    position.y == node.position.y);
}

AStar::AStar()
{
  base_step_cost_ = 10;
  actual_state_ = AStarStatus::k_Finished;
}

AStar::~AStar()
{
  clean();
}

s16 AStar::generatePath(Float2 origin, Float2 dst,Path* path, const Map& collisionData)
{
  printf("Calculating path... wait please \n");
  Float2 origin_ratio = origin / collisionData.ratio();
  Float2 dst_ratio = dst / collisionData.ratio();

  origin_ratio.x = floorf(origin_ratio.x);
  origin_ratio.y = floorf(origin_ratio.y);

  dst_ratio.x = floorf(dst_ratio.x);
  dst_ratio.y = floorf(dst_ratio.y);


  if (!path) return kErrorCode_InvalidPointer;
  //Check that the origin and destination are valid in our map coordinates
  if (collisionData.isOccupied(origin_ratio.x, origin_ratio.y))
  {
    printf("Trying to reach an invalid position \n");
    return kErrorCode_PathNotFound;
  }
  if (collisionData.isOccupied(dst_ratio.x, dst_ratio.y))
  {
    printf("Trying to reach an invalid position \n");
    return kErrorCode_PathNotFound;
  }
  //path->clear();

  //Create a node containing the goal state: node_goal
  AStarNode* node_goal = new AStarNode(dst_ratio, nullptr, base_step_cost_);

  if (node_goal == nullptr) return kErrorCode_Memory;

  //Create a node containing the start state: node_start
  AStarNode* node_start = new AStarNode(origin_ratio, nullptr, base_step_cost_);

  if (node_start == nullptr)
  {
    delete node_goal;
    return kErrorCode_Memory;
  }

  open_list_.clear();

  //Put node_start on the OPEN list
  open_list_.push_back(node_start);

  AStarNode* node_current = nullptr;

  //while the OPEN list is not empty
  while(!open_list_.empty())
  {
    //Get the node off the OPEN list with the lowest f and call it node_current
    const s32 lowest_f_idx = getLowestFNodeIdx();
    node_current = open_list_[lowest_f_idx];
    open_list_.erase(open_list_.begin() + lowest_f_idx);


    //If node_current is the same state as node_goal: break from the while loop
    if (node_current->hasSameState(*node_goal)) break;

    //Generate each state node_successor that can come after node_current
    for (AgentDirection d : g_directions)
    {
      bool finished_loop = false;
      u16 step_cost = base_step_cost_;
      Float2 new_position = node_current->position;
      switch (d)
      {
      case AgentDirection::k_Northwest:
        new_position = Float2(new_position.x - 1, new_position.y - 1);
        step_cost += 5;
        break;
      case AgentDirection::k_North:
        new_position = Float2(new_position.x , new_position.y - 1);
        break;
      case AgentDirection::k_Northeast:
        new_position = Float2(new_position.x + 1, new_position.y - 1);
        step_cost += 5;
        break;
      case AgentDirection::k_West:
        new_position = Float2(new_position.x - 1, new_position.y);
        break;
      case AgentDirection::k_East:
        new_position = Float2(new_position.x + 1, new_position.y);
        break;
      case AgentDirection::k_Southwest:
        new_position = Float2(new_position.x - 1, new_position.y + 1);
        step_cost += 5;
        break;
      case AgentDirection::k_South:
        new_position = Float2(new_position.x, new_position.y + 1);
        break;
      case AgentDirection::k_Southeast:
        new_position = Float2(new_position.x + 1, new_position.y + 1);
        step_cost += 5;
        break;
      default:
        printf("Problem at A*!!!!!! \n");
        delete node_goal;
        //We don't need to delete node_start as right now it is already on the lists
        //that will be cleaned at clean()
        //delete node_start;
        delete node_current;
        clean();
        return kErrorCode_PathNotCreated;
      }
      //If the position obtained is not occupied (in case is invalid counts as if it's occupied)
      if(!collisionData.isOccupied(new_position.x, new_position.y))
      {
        //TODO check if it's worth to only creating it after checking if it's in the open list or the closed list
        AStarNode* node_successor = new AStarNode(Float2(new_position.x, new_position.y), node_current, step_cost);
        s32 idx_ol = -1;
        s32 idx_cl = -1;
        /*If node_successor is on the OPEN list but the existing one is as good or
        better then discard this successor and continue with next successor*/
        if(isNodeInOpenList(new_position.x, new_position.y, &idx_ol))
        {
          if(open_list_[idx_ol]->g <= node_successor->g)
          {
            finished_loop = true;
            delete node_successor;
            //continue;
          }
        }
        /*If node_successor is on the CLOSED list  but the existing one is as good or
        better then discard this successor and continue with next successor*/
        if(!finished_loop && isNodeInClosedList(new_position.x, new_position.y, &idx_cl))
        {
          if (closed_list_[idx_cl]->g <= node_successor->g)
          {
            finished_loop = true;
            delete node_successor;
            //continue;
          }
        }
        if(!finished_loop)
        {
          //Remove occurences of node_successor from OPEN and CLOSED
          if(idx_ol != -1)
          {
            AStarNode* aux = open_list_[idx_ol];
            open_list_.erase(open_list_.begin() + idx_ol);
            delete aux;
          }
          if(idx_cl != -1)
          {
            AStarNode* aux = closed_list_[idx_cl];
            closed_list_.erase(closed_list_.begin() + idx_cl);
            delete aux;
          }
          //Set the parent of node_successor to node_current
          node_successor->parent = node_current;
          //Set h to be the estimated distance to node_goal (using the heuristic function)
          node_successor->h = calculateHeuristic(node_successor->position, node_goal->position);
          node_successor->f = node_successor->g + node_successor->h;
          //Add node_successor to the OPEN list
          open_list_.push_back(node_successor);
        }
      }
    }
    //Add node_current to the CLOSED list
    closed_list_.push_back(node_current);
  }

  //If the current node is not the node goal we didn't find the path
  if (!node_current->hasSameState(*node_goal)) {
    //We don't need to delete node_start as right now it is already on the lists
    //that will be cleaned at clean()
    //delete node_start;
    delete node_goal;
    clean(); printf("Path not found.\n");
    return kErrorCode_PathNotFound;
  }

  /*Once we got the path calculated we create it using our path class*/
  //We count the number of points needed for the path
  AStarNode* aux = node_current;
  u16 count = 0;
  std::stack<AStarNode*> final_path;
  final_path.push(aux);
  count++;
  while(!aux->hasSameState(*node_start))
  {
    aux = aux->parent;
    final_path.push(aux);
    count++;
  }
  path->create(count);
  while (!final_path.empty())
  {
    AStarNode* tmp = final_path.top();
    final_path.pop();
    path->addPoint(tmp->position.x * collisionData.ratio().x, tmp->position.y * collisionData.ratio().y);
  }
  path->set_direction(Direction::kDirForward);
  path->setToReady();

  //We don't need to delete node_start as right now it is already on the lists
  //that will be cleaned at clean()
  //delete node_start;
  delete node_goal;
  clean(); 
  printf(" Path found, please press F2 to start.\n");
  return kErrorCode_Ok;
}

s16 AStar::generatePath(Float2 origin, Float2 dst, Path* path, const Map& collisionData, double timeout)
{
  const double start_time = ESAT::Time();
  double elapsed_time = 0;
  if(actual_state_ == AStarStatus::k_Finished)
  {
    printf("Calculating path... wait please \n");
    Float2 origin_ratio = origin / collisionData.ratio();
    Float2 dst_ratio = dst / collisionData.ratio();

    origin_ratio.x = floorf(origin_ratio.x);
    origin_ratio.y = floorf(origin_ratio.y);

    dst_ratio.x = floorf(dst_ratio.x);
    dst_ratio.y = floorf(dst_ratio.y);

    if (!path) return kErrorCode_InvalidPointer;
    //Check that the origin and destination are valid in our map coordinates
    if (collisionData.isOccupied(origin_ratio.x, origin_ratio.y))
    {
      printf("Trying to reach an invalid position \n");
      return kErrorCode_PathNotFound;
    }
    if (collisionData.isOccupied(dst_ratio.x, dst_ratio.y))
    {
      printf("Trying to reach an invalid position \n");
      return kErrorCode_PathNotFound;
    }
    //path->clear();

    //Create a node containing the goal state: node_goal
    node_goal = new AStarNode(dst_ratio, nullptr, base_step_cost_);

    if (node_goal == nullptr) return kErrorCode_Memory;

    //Create a node containing the start state: node_start
    node_start = new AStarNode(origin_ratio, nullptr, base_step_cost_);

    if (node_start == nullptr)
    {
      delete node_goal;
      return kErrorCode_Memory;
    }

    open_list_.clear();

    //Put node_start on the OPEN list
    open_list_.push_back(node_start);

    actual_state_ = AStarStatus::k_Calculating;
    node_current = nullptr;
  }

  //while the OPEN list is not empty
  while (!open_list_.empty() && !(elapsed_time > timeout))
  {
    //Get the node off the OPEN list with the lowest f and call it node_current
    const s32 lowest_f_idx = getLowestFNodeIdx();
    node_current = open_list_[lowest_f_idx];
    open_list_.erase(open_list_.begin() + lowest_f_idx);


    //If node_current is the same state as node_goal: break from the while loop
    if (node_current->hasSameState(*node_goal)) break;

    //Generate each state node_successor that can come after node_current
    for (AgentDirection d : g_directions)
    {
      bool finished_loop = false;
      u16 step_cost = base_step_cost_;
      Float2 new_position = node_current->position;
      switch (d)
      {
      case AgentDirection::k_Northwest:
        new_position = Float2(new_position.x - 1, new_position.y - 1);
        step_cost += 5;
        break;
      case AgentDirection::k_North:
        new_position = Float2(new_position.x, new_position.y - 1);
        break;
      case AgentDirection::k_Northeast:
        new_position = Float2(new_position.x + 1, new_position.y - 1);
        step_cost += 5;
        break;
      case AgentDirection::k_West:
        new_position = Float2(new_position.x - 1, new_position.y);
        break;
      case AgentDirection::k_East:
        new_position = Float2(new_position.x + 1, new_position.y);
        break;
      case AgentDirection::k_Southwest:
        new_position = Float2(new_position.x - 1, new_position.y + 1);
        step_cost += 5;
        break;
      case AgentDirection::k_South:
        new_position = Float2(new_position.x, new_position.y + 1);
        break;
      case AgentDirection::k_Southeast:
        new_position = Float2(new_position.x + 1, new_position.y + 1);
        step_cost += 5;
        break;
      default:
        printf("Problem at A*!!!!!! \n");
        delete node_goal;
        //We don't need to delete node_start as right now it is already on the lists
        //that will be cleaned at clean()
        //delete node_start;
        delete node_current;
        clean();
        return kErrorCode_PathNotCreated;
      }
      //If the position obtained is not occupied (in case is invalid counts as if it's occupied)
      if (!collisionData.isOccupied(new_position.x, new_position.y))
      {
        //TODO check if it's worth to only creating it after checking if it's in the open list or the closed list
        AStarNode* node_successor = new AStarNode(Float2(new_position.x, new_position.y), node_current, step_cost);
        s32 idx_ol = -1;
        s32 idx_cl = -1;
        /*If node_successor is on the OPEN list but the existing one is as good or
        better then discard this successor and continue with next successor*/
        if (isNodeInOpenList(new_position.x, new_position.y, &idx_ol))
        {
          if (open_list_[idx_ol]->g <= node_successor->g)
          {
            finished_loop = true;
            delete node_successor;
            //continue;
          }
        }
        /*If node_successor is on the CLOSED list  but the existing one is as good or
        better then discard this successor and continue with next successor*/
        if (!finished_loop && isNodeInClosedList(new_position.x, new_position.y, &idx_cl))
        {
          if (closed_list_[idx_cl]->g <= node_successor->g)
          {
            finished_loop = true;
            delete node_successor;
            //continue;
          }
        }
        if (!finished_loop)
        {
          //Remove occurences of node_successor from OPEN and CLOSED
          if (idx_ol != -1)
          {
            AStarNode* aux = open_list_[idx_ol];
            open_list_.erase(open_list_.begin() + idx_ol);
            delete aux;
          }
          if (idx_cl != -1)
          {
            AStarNode* aux = closed_list_[idx_cl];
            closed_list_.erase(closed_list_.begin() + idx_cl);
            delete aux;
          }
          //Set the parent of node_successor to node_current
          node_successor->parent = node_current;
          //Set h to be the estimated distance to node_goal (using the heuristic function)
          node_successor->h = calculateHeuristic(node_successor->position, node_goal->position);
          node_successor->f = node_successor->g + node_successor->h;
          //Add node_successor to the OPEN list
          open_list_.push_back(node_successor);
        }
      }
    }
    //Add node_current to the CLOSED list
    closed_list_.push_back(node_current);
    elapsed_time =  ESAT::Time() - start_time;
  }
  if(elapsed_time > timeout)
  {
    return kErrorCode_Timeout;
  }

  //If the current node is not the node goal we didn't find the path
  if (!node_current->hasSameState(*node_goal)) {
    //We don't need to delete node_start as right now it is already on the lists
    //that will be cleaned at clean()
    //delete node_start;
    delete node_goal;
    clean(); 
    printf("Path not found.\n");
    actual_state_ = AStarStatus::k_Finished;
    return kErrorCode_PathNotFound;
  }

  /*Once we got the path calculated we create it using our path class*/
  //We count the number of points needed for the path
  AStarNode* aux = node_current;
  u16 count = 0;
  std::stack<AStarNode*> final_path;
  final_path.push(aux);
  count++;
  while (!aux->hasSameState(*node_start))
  {
    aux = aux->parent;
    final_path.push(aux);
    count++;
  }
  path->create(count);
  while (!final_path.empty())
  {
    AStarNode* tmp = final_path.top();
    final_path.pop();
    path->addPoint(tmp->position.x * collisionData.ratio().x, tmp->position.y * collisionData.ratio().y);
  }
  path->set_direction(Direction::kDirForward);
  path->setToReady();
  printf(" Path found, proceeding to execute it.\n");
  //We don't need to delete node_start as right now it is already on the lists
  //that will be cleaned at clean()
  //delete node_start;
  delete node_goal;
  clean();
  actual_state_ = AStarStatus::k_Finished;
  return kErrorCode_Ok;
}

void AStar::clean()
{
  while(!open_list_.empty()){
    AStarNode* aux = open_list_.back();
    open_list_.pop_back();
    delete aux;
  }
  while (!closed_list_.empty()) {
    AStarNode* aux = closed_list_.back();
    closed_list_.pop_back();
    delete aux;
  }
}

u32 AStar::calculateHeuristic(const Float2& origin, const Float2& dst) const
{
  const Float2 result = dst - origin;
  //L infinite
  u32 quantity_moved = static_cast<u32>(fmax(result.x, result.y));
  //L
  //u32 quantity_moved = result.Length();
  quantity_moved *= base_step_cost_;
  return quantity_moved;
}

bool AStar::isNodeInClosedList(const float x, const float y, s32* position)
{
  s32 idx = 0;
  for (AStarNode* node : closed_list_)
  {
    if (node->position == Float2(x, y))
    {
      *position = idx;
      return true;
    }
    idx++;
  }
  *position = -1;
  return false;
}

bool AStar::isNodeInOpenList(const float x, const float y, s32* position)
{
  s32 idx = 0;
  for (AStarNode* node : open_list_)
  {
    if (node->position == Float2(x, y))
    {
      *position = idx;
      return true;
    }
    idx++;
  }
  *position = -1;
  return false;
}

s32 AStar::getLowestFNodeIdx() const
{
  if (open_list_.empty()) return -1;
  s32 idx = 0;
  s32 result = 0;
  u32 current_f = open_list_[0]->f;

  for (AStarNode* node : open_list_)
  {
    if (current_f > node->f)
    {
      result = idx;
      current_f = node->f;
    }
    idx++;
  }
  return result;
}