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

class Float2;
const int kMaxPoints = 500;

enum class Direction
{
  kDirNone = -1,
  kDirForward = 0
};

enum class Action
{
  kActionNone = -1,
  kActionStraight = 0,
  kActionLoopNTimes,
  kActionLoopInfinite
};
/** @brief Path class
*
* Class in charge of storing sets of points, it also stores rules
* to indicate how we want to traverse those points.
*
*/
class Path
{
public:
  /** @brief Path constructor
  *
  * Constructor of path. Sets the state to a default one, note that the path is not ready
  * after the construcotr.
  *
  * @return *Agent
  */
  Path();
  /** @brief Destroys the Path
  *
  * Destructor of the path
  *
  * @return void
  */
  ~Path();
  /** @brief creates a Path
  *
  * Allocates the memory that the path will have. Note that passing 0 points ore more than the
  * kMaxPoints will result in a kErrorCode_IncorrectPointsNumber. Otherwise if all went correct
  * a kErrorCode_Ok will be returned. This function clears and initializes any data that was stored
  * in the path previously if any.
  *
  * @param points number of points that will have the path
  * @return s16 result of the operation
  */
  s16 create(u16 points = 50);
  /** @brief sets the path to a ready state
  *
  * Checks that everything was correctly initialized, if that's the case sets the path to ready and
  * from that point the path can be used. Please check the error code to know more info about the
  * reason why the operation fails.
  * kErrorCode_EmptyPath -> The path is empty.
  * kErrorCode_PathNotCreated -> The path has not been created. Make sure you called create() 
  *   before setToReady
  * kErrorCode_BadLoopsSetting -> The loop setting has not been correctly configured.
  * kErrorCode_BadDirectionSetting -> The direction setting has not been correctly configured.
  *
  * @return s16 result of the operation
  */
  s16 setToReady();
  /** @brief sets the direction of the path
  *
  * Sets the direction of the path:
  * kDirNone -> Invalid direction
  * kDirForward -> The path will go from the first point to the last one
  *
  * @param direction that will use the path
  * @return s16 result of the operation
  */
  s16 set_direction(Direction direction);
  /** @brief sets the action of the path
  *
  * Sets the action of the path:
  * kActionNone  -> setting with no loops
  * kActionStraight -> setting with no loops
  * kActionLoopInfinite -> Once the path is finished it will start from the start again
  * kActionLoopNTimes -> Will result in an error kErrorCode_BadLoopsSetting, please
  *   use the set action function that accepts loops
  *
  * @param action that will use the path
  * @return s16 result of the operation
  */
  s16 set_action(Action action);
  /** @brief sets the action of the path
  *
  * Sets the action of the path. Use this function to set a kActionLoopNTimes. 
  * Otherwise the function will return an error.
  * kActionLoopNTimes -> Once the path is finished it will start from the start again N times
  *
  * @param action that will use the path
  * @param loops the number of times the path will be repeated
  * @return s16 result of the operation
  */
  s16 set_action(Action action, s16 loops);
  /** @brief adds a new point to the path
  *
  * Adds a new point to the path. Note that if the path is already full or has not been
  * created this function will return an error
  *
  * @param x x coordinate of the point
  * @param y y coordinate of the point
  * @return s16 result of the operation
  */
  s16 addPoint(float x, float y);
  /** @brief adds a new point to the path
  *
  * Adds a new point to the path. Note that if the path is already full or has not been
  * created this function will return an error
  *
  * @param new_point point to be added
  * @return s16 result of the operation
  */
  s16 addPoint(Float2 *new_point);
  /** @brief informs if the current point of the path is the las one
  *
  * Informs if the current point of the path is the las one. True if it is the last one
  * false otherwise
  *
  * @return bool result of the operation
  */
  bool isLast();
  /** @brief informs if the path is ready to be used
  *
  * Informs if the path is ready to be used. True if it is the last one
  * false otherwise
  *
  * @return bool result of the operation
  */
  bool isReady();
  /** @brief Gets the next point of the path.
  *
  * Gets the next point of the path. In case the path is not ready or
  * there is no next point it will return nullptr.
  *
  * @return Float2 next point of the path
  */
  Float2 const* nextPoint();
  /** @brief Gets the prev point of the path.
  *
  * Gets the prev point of the path.In case the path is not ready or
  * there is no prev point it will return nullptr.
  *
  * @return Float2 prev point of the path
  */
  Float2 const* prevPoint();
  /** @brief Gets the last point of the path.
  *
  * Gets the last point of the path. In case the path is not ready 
  * it will return nullptr.
  *
  * @return Float2 last point of the path
  */
  Float2 const* lastPoint();

  u16 total_points_;
private:
  Float2 *points_;

  Direction direction_;
  Action action_;

  bool ready_;

  s16 cp_index_; // current point index_
  s16 lp_index_; // last point index
  

  s16 num_loops_; // -1 = infinite, 0 = no loops, n = n loops
  u16 current_loop_;
  s16 offset_;
  /** @brief Path copy constructor
  *
  * The Path cannot be copied
  *
  * @return *Path
  */
  Path(const Path& p) = delete;
  /** @brief Path copy operation
  *
  * The Path cannot be copied
  *
  * @return *Path
  */
  Path operator=(const Path& p) = delete;
  /** @brief initializes the path
  *
  * Sets the path to an initialized state ready for reuse
  *
  * @return s16 result of the operation
  */
  s16 init();
  /** @brief clears the allocated memory
  *
  * Clears the allocated memory. After this operation the path won't be ready anymore, so a setToReady
  * will be needed again.
  *
  * @return s16 result of the operation
  */
  s16 clear();
};

#endif
