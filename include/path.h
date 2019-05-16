/**
*  \file path.h
*
*  \autor Toni Barella
*  Artificial Intelligence - 3HND
*   ESAT 2016 - 2019
*   
*   \autor Jose Maria Martinez
*
*  \brief Class to construct and walk trough a path
*/

#ifndef __PATH_H__
#define __PATH_H__

#include "platform_types.h"
#include "common_def.h"
#include <cstdio>

class Float2;
const int kMaxPoints = 500;

enum class Direction
{
  kDirNone = -1,
  kDirForward = 0,
  kDirBackward
};

enum class Action
{
  kActionNone = -1,
  kActionStraight = 0,
  kActionLoopNTimes,
  kActionLoopInfinite
};

class Path
{
public:
  Path();
  ~Path();

  s16 create(u16 points = 50);
  s16 clear();
  s16 setToReady();

  s16 set_direction(Direction direction);
  s16 set_action(Action action);
  s16 set_action(Action action, s16 loops);


  //s16 addPoint(float x, float y, float z);
  s16 addPoint(float x, float y);
  //s16 addPoint(Vector3 *new_point);
  s16 addPoint(Float2 *new_point);
  bool isLast();
  bool isReady();

  //Vector3 const* nextPoint();
  //Vector3 const* prevPoint();
  //Vector3 const* lastPoint();
  Float2 const* nextPoint();
  Float2 const* prevPoint();
  Float2 const* lastPoint();

  s16 print(FILE *out_file = stdout); // not only the list of points, but other features too (direction, loops, etc.)

private:
  Float2 *points_;
  /*Vector3 *points_;*/
  //Vector3 points_[kMaxPoints];
  //Curse2Vector *points_;

  Direction direction_;
  Action action_;

  bool ready_;

  s16 cp_index_; // current point index_
  s16 lp_index_; // last point index
  s16 total_points_;

  s16 num_loops_; // -1 = infinite, 0 = no loops, n = n loops
  u16 current_loop_;
  s16 offset_;

  Path(const Path& p) = delete;
  Path operator=(const Path& p) = delete;
};

#endif
