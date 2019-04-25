#ifndef __PATH_FINDER_H__
#define __PATH_FINDER_H__

#include "astar.h"

class Path;

class PathFinder
{
public:
  PathFinder();
  ~PathFinder();

  s16 LoadMap(const char* src, const float original_map_width, const float original_map_height);

  s16 GeneratePath(Path* path);

  void update(const uint32_t dt);

private:
  MapData current_map_;

  //TODO make sure there is no path being calculated
  //bool calculating_path_;

  AStar* a_star_;

  PathFinder(const PathFinder& pf) {};
  PathFinder operator=(const PathFinder& pf) {};
};

#endif