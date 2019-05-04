#include "astar.h"
#include "path.h"
#include <stack>

static const AgentDirection g_directions[8] = { AgentDirection::k_Northwest, AgentDirection::k_North, AgentDirection::k_Northeast,
                                              AgentDirection::k_West, AgentDirection::k_East,
                                              AgentDirection::k_Southwest, AgentDirection::k_South, AgentDirection::k_Southeast };

AStarNode::AStarNode(Float2 position, AStarNode* parent, s32 step)
{
  position_ = position;
  parent_ = parent;
  if(parent_)
  {
    g = parent->g + step;
    h = 0;
    f = g + h;
  }else
  {
    g = 0;
    h = 0;
    f = 0;
  }
  
}

AStarNode::~AStarNode()
{
  
}

bool AStarNode::HasSameState(const AStarNode& node)
{
  return (position_.x == node.position_.x &&
    position_.y == node.position_.y);
}

AStar::AStar()
{
  base_step_cost_ = 10;
}

AStar::~AStar()
{
  clean();
}

s16 AStar::generatePath(Float2 origin, Float2 dst,Path* path, const MapData& collisionData)
{
  printf("Calculating path... wait please \n");
  Float2 origin_ratio = origin / collisionData.ratio_;
  Float2 dst_ratio = dst / collisionData.ratio_;

  origin_ratio.x = static_cast<u32>(origin_ratio.x);
  origin_ratio.y = static_cast<u32>(origin_ratio.y);

  dst_ratio.x = static_cast<u32>(dst_ratio.x);
  dst_ratio.y = static_cast<u32>(dst_ratio.y);

  if (!path) return kErrorCode_InvalidPointer;
  //Check that the origin and destination are valids in our map coordinates
  if (!collisionData.IsValidPosition(origin_ratio.x, origin_ratio.y))
  {
    return kErrorCode_InvalidOrigin;
  }
  if (!collisionData.IsValidPosition(dst_ratio.x, dst_ratio.y))
  {
    return kErrorCode_InvalidDestination;
  }
  path->clear();

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
    if (node_current->HasSameState(*node_goal)) break;

    //Generate each state node_successor that can come after node_current
    for (AgentDirection d : g_directions)
    {
      bool finished_loop = false;
      u16 step_cost = base_step_cost_;
      Float2 new_position = node_current->position_;
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
      if(!collisionData.IsOccupied(new_position.x, new_position.y))
      {
        //TODO check if it's worth to only creating it after checking if it's in the open list or the closed list
        AStarNode* node_successor = new AStarNode(Float2(new_position.x, new_position.y), node_current, step_cost);
        s32 idx_ol;
        s32 idx_cl;
        /*If node_successor is on the OPEN list but the existing one is as good or
        better then discard this successor and continue with next successor*/
        if(isNodeInOpenList(new_position.x, new_position.y, &idx_ol))
        {
          if(open_list_[idx_ol]->g <= node_successor->g)
          {
            finished_loop = true;
            delete node_successor;
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
          node_successor->parent_ = node_current;
          //Set h to be the estimated distance to node_goal (using the heuristic function)
          node_successor->h = calculateHeuristic(node_successor);
          //Add node_successor to the OPEN list
          open_list_.push_back(node_successor);
        }
      }
    }
    //Add node_current to the CLOSED list
    closed_list_.push_back(node_current);
  }

  //If the current node is not the node goal we didn't find the path
  if (!node_current->HasSameState(*node_goal)) {
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
  u32 count = 0;
  std::stack<AStarNode*> final_path;
  final_path.push(aux);
  count++;
  while(!aux->HasSameState(*node_start))
  {
    aux = aux->parent_;
    final_path.push(aux);
    count++;
  }
  path->create(count);
  while (!final_path.empty())
  {
    AStarNode* tmp = final_path.top();
    final_path.pop();
    path->addPoint(tmp->position_.x * collisionData.ratio_.x, tmp->position_.y * collisionData.ratio_.y);
  }
  path->set_direction(Direction::kDirForward);
  path->setToReady();

  clean();
  //We don't need to delete node_start as right now it is already on the lists
  //that will be cleaned at clean()
  //delete node_start;
  delete node_goal;
  clean(); 
  printf(" Path found, please press F1 to start.\n");
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

u32 AStar::calculateHeuristic(const AStarNode* node)
{
  return 0;
}

bool MapData::IsValidPosition(const s32 x, const s32 y) const
{
  if (x > width_ || x < 0) return false;
  if (y > height_ || y < 0) return false;
  return true;
}

bool MapData::IsOccupied(const s32 x, const s32 y) const
{
  if (!IsValidPosition(x, y)) return true;
  const s32 position = x+(y * width_);
  return !collision_data_[position];
}

bool AStar::isNodeInClosedList(const s32 x, const s32 y, s32* position)
{
  s32 idx = 0;
  for (AStarNode* node : closed_list_)
  {
    if (node->position_ == Float2(x, y))
    {
      *position = idx;
      return true;
    }
    idx++;
  }
  *position = -1;
  return false;
}

bool AStar::isNodeInOpenList(const s32 x, const s32 y, s32* position)
{
  s32 idx = 0;
  for (AStarNode* node : open_list_)
  {
    if (node->position_ == Float2(x, y))
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
  s32 current_f = open_list_[0]->f;

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