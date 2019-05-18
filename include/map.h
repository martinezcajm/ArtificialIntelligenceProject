// map.h
// Jose Maria Martinez
// Header of the functions of the map class
#ifndef __MAP_H__
#define __MAP_H__

#include "ESAT/sprite.h"
#include "platform_types.h"
#include "Math/float2.h"
/** @brief Map class
*
* Class in charge of storing the background representation of the map
* and it's collision data
*
*/
class Map
{
public:
  /** @brief Map constructor
  *
  * Default Map constructor
  *
  * @return *Map
  */
  Map();
  /** @brief Map destructor
  *
  * Default Map destructor
  *
  * @return *Map
  */
  ~Map();
  /** @brief checks if a position is a valid one in the map
  *
  * Checks if a position is a valid one in the map
  *
  * @param x x coordinate of the position we want to check
  * @param y y coordinate of the position we want to check
  * @return bool true if it is valid false otherwise
  */
  bool isValidPosition(const float x, const float y) const;
  /** @brief checks if a position is occupied in the map
  *
  * Checks if a position is occupied in the map
  *
  * @param x x coordinate of the position we want to check
  * @param y y coordinate of the position we want to check
  * @return bool true if it is valid false otherwise
  */
  bool isOccupied(const float x, const float y) const;
  /** @brief Loads the map
  *
  * Tries to load a map with an image source for the background and an image source
  * for the collisions ( white = free, black = occupied). If another map was already
  * loaded it will be freed. The possible results of this operation are:
  * kErrorCode_InvalidPointer -> The source or the background were nullptr
  * kErrorCode_Memory -> The program was unable of allocating memory
  * kErrorCode_Ok -> Everything went fine
  *
  * @param src x coordinate of the position we want to check
  * @param background y coordinate of the position we want to check
  * @return status of the operation
  */
  s16 loadMap(const char* src, const char* background);
  /** @brief returns the ratio between the original map and the collisions map
  *
  * Returns the ratio between the original map and the collisions map
  *
  * @return Float2 ratio
  */
  Float2 ratio() const;
  /** @brief returns the image of the original map
  *
  * Returns the image of the original map
  *
  * @return SpriteHandle image of the background
  */
  ESAT::SpriteHandle background() const;

private:
  /** @brief frees the allocated resources
  *
  * frees the allocated resources
  *
  * @return void
  */
  void freeResources();
  /** @brief Map copy constructor
  *
  * The Map cannot be copied
  *
  * @return *Map
  */
  Map(const Map& map) = delete;

  ESAT::SpriteHandle background_;

  s32 width_;

  s32 height_;

  s32 original_width_;

  s32 original_height_;

  bool* collision_data_;

  Float2 ratio_;

  
};

#endif