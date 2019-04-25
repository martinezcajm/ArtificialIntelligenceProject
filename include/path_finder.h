#ifndef __PATH_FINDER_H__
#define __PATH_FINDER_H__

#include "common_def.h"
#include "platform_types.h"

class Path;

typedef struct {
  s32 width;
  s32 height;
  bool* collisionData;
} MapData;

class PathFinder
{
public:
  PathFinder();
  ~PathFinder();

  s16 LoadMap(const char* src);

  s16 GeneratePath(Path* path);

private:
  MapData current_map_;

  PathFinder(const PathFinder& pf) {};
  PathFinder operator=(const PathFinder& pf) {};
};

#endif