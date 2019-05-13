#include "path.h"
#include "Math/float2.h"
#include <cstdlib>

Path::Path()
{
  ready_ = false;
  direction_ = Direction::kDirNone;
  action_ = Action::kActionNone;
  lp_index_ = -1;
  cp_index_ = -1;
  total_points_ = 0;
  points_ = nullptr;
  num_loops_ = 0;
  current_loop_ = 0;
}

Path::~Path()
{
  clear();
}

s16 Path::create(u32 points)
{
  if (points == 0 || points > kMaxPoints) return kErrorCode_IncorrectPointsNumber;
  total_points_ = static_cast<s16>(points);
  //points_ = static_cast<Vector3*>( malloc(total_points_ * sizeof(Vector3)));
  points_ = static_cast<Float2*>(malloc(total_points_ * sizeof(Vector3)));
  return kErrorCode_Ok;
}

s16 Path::clear()
{
  if(points_)
  {
    free(points_);
  }
  return kErrorCode_Ok;
}

s16 Path::setToReady()
{
  if (!points_) return kErrorCode_PathNotCreated;
  if (-1 == lp_index_) return kErrorCode_EmptyPath;
  if (num_loops_ < -1) return kErrorCode_BadLoopsSetting;
  if (direction_ == Direction::kDirNone) return kErrorCode_BadDirectionSetting;
  ready_ = true;
  return kErrorCode_Ok;
}

s16 Path::set_direction(Direction direction)
{
  direction_ = direction;
  return kErrorCode_Ok;
}

s16 Path::set_action(Action action)
{
  switch (action)
  {
  case Action::kActionLoopInfinite:
    num_loops_ = -1;
    break;
  case Action::kActionLoopNTimes:
    return kErrorCode_BadLoopsSetting;
  default:
    num_loops_ = 0;
    break;
  }
  action_ = action;
  return kErrorCode_Ok;
}

s16 Path::set_action(Action action, s16 loops)
{
  if (loops < 0 && action != Action::kActionLoopNTimes) return kErrorCode_BadLoopsSetting;
  action_ = action;
  num_loops_ = loops;
  return kErrorCode_Ok;
}

//s16 Path::addPoint(Vector3* new_point)
//{
//  if (!new_point) return kErrorCode_InvalidPointer;
//  if (isLast()) return kErrorCode_StorageFull;
//  lp_index_++;
//  *(points_ + lp_index_) = *new_point;
//  //Vector3* aux = points_ + lp_index_;
//  //*aux = *new_point;
//  return kErrorCode_Ok;
//}

s16 Path::addPoint(Float2* new_point)
{
  if (!new_point) return kErrorCode_InvalidPointer;
  if (isLast()) return kErrorCode_StorageFull;
  lp_index_++;
  *(points_ + lp_index_) = *new_point;
  //Vector3* aux = points_ + lp_index_;
  //*aux = *new_point;
  return kErrorCode_Ok;
}

//s16 Path::addPoint(float x, float y, float z)
//{
//  if (isLast()) return kErrorCode_StorageFull;
//  lp_index_++;
//  Vector3* aux = points_ + lp_index_;
//  aux->x = x;
//  aux->y = y;
//  aux->z = z;
//  return kErrorCode_Ok;
//}

s16 Path::addPoint(float x, float y)
{
  if (isLast()) return kErrorCode_StorageFull;
  lp_index_++;
  Float2* aux = points_ + lp_index_;
  aux->x = x;
  aux->y = y;
  return kErrorCode_Ok;
}

bool Path::isLast()
{
  return total_points_ == cp_index_ + 1;
}

bool Path::isReady()
{
  return ready_;
}

Float2 const* Path::nextPoint()
{
  //if (!isReady())return;
  //In case this is the last point and we are doing an infinite loop or
  //this is still is not the last loop we start again
  if(isLast())
  {
    if(num_loops_ == -1 || static_cast<s16>(current_loop_) < num_loops_)
    {
      current_loop_++;
      cp_index_ = -1;
    }
  }
  cp_index_++;
  const Float2* v = points_ + cp_index_;
  return v;
}

Float2 const* Path::prevPoint()
{
  if (cp_index_ == 0)
  {
    //In case this is the first point and we already made a loop we go to the last point
    //as it was the previous one
    if (static_cast<s16>(current_loop_) > 0)
    {
      current_loop_--;
      cp_index_ = lp_index_ + 1;
    }
  }
  cp_index_--;
  //const Float2* v = points_ + cp_index_;
  return points_ + cp_index_;
}

Float2 const* Path::lastPoint()
{
  //if (!isReady())return const Float2*{0.0f,0.0f};
  return points_ + lp_index_;
}

s16 Path::print(FILE* out_file)
{
  return kErrorCode_Ok;
}
