#ifndef __PATH_FINDER_H__
#define __PATH_FINDER_H__

#include "astar.h"
#include "map.h"

class Path;
class Map;

class PathFinder
{
public:
  PathFinder::PathFinder(Map& map);
  ~PathFinder();

  s16 GeneratePath(Path* path, Float2 origin, Float2 dst);

  void update(const uint32_t dt);

private:
  //TODO make sure there is no path being calculated
  //bool calculating_path_;

  AStar* a_star_;

  PathFinder(const PathFinder& pf)  = delete;
  PathFinder operator=(const PathFinder& pf) = delete;
};

#endif