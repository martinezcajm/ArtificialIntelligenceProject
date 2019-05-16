#ifndef __ASTAR_H__
#define __ASTAR_H__

#include "platform_types.h"
#include "common_def.h"
#include "Math/float2.h"
#include <vector>

class Map;
class Path;

//typedef struct MapData {
//  s32 width_;
//  s32 height_;
//  bool* collision_data_;
//  Float2 ratio_;
//  bool IsValidPosition(const s32 x, const s32 y) const;
//  bool IsOccupied(const s32 x, const s32 y) const;
//};

typedef struct AStarNode
{
  Float2 position_;
  u16 f;
  u16 g;
  u16 h;
  AStarNode* parent_;

  AStarNode(Float2 position, AStarNode* parent, s32 step);
  ~AStarNode();
  bool HasSameState(const AStarNode& node);
};

enum class AgentDirection
{
  k_East = 0,
  k_West = 1,
  k_South = 2,
  k_North = 3,
  k_Northwest = 4,
  k_Northeast = 5,
  k_Southwest = 6,
  k_Southeast = 7,
  k_PADDING = 255
};

class AStar
{
public:
  AStar();

  ~AStar();

  s16 generatePath(Float2 origin, Float2 dst, Path* path, const Map& collisionData);

private:

  std::vector<AStarNode*> open_list_;

  std::vector<AStarNode*> closed_list_;

  u16 base_step_cost_;

  AStar(const AStar& as) {};
  AStar operator=(const AStar& as) {};

  bool isNodeInClosedList(const s32 x, const s32 y, s32* position);

  bool isNodeInOpenList(const s32 x, const s32 y, s32* position);

  s32 getLowestFNodeIdx() const;

  void clean();

  u32 calculateHeuristic(const Float2& origin, const Float2& dst) const;

  //void ExpandNodeInDirection(AgentDirection d, AStarNode* node, const MapData& collisionData);

};

#endif