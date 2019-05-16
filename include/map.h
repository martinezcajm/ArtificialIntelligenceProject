#ifndef __MAP_H__
#define __MAP_H__

#include "ESAT/sprite.h"
#include "platform_types.h"
#include "Math/float2.h"

class Map
{
public:
  Map();

  ~Map();

  bool isValidPosition(const float x, const float y) const;

  bool isOccupied(const float x, const float y) const;

  s16 loadMap(const char* src, const char* background);

  Float2 ratio() const;

  ESAT::SpriteHandle background() const;

private:

  void freeResources();

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