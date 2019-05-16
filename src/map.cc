#include "map.h"
#include "STB/stb_image.h"
#include "common_def.h"
#include <cstdlib>


Map::Map()
{
  height_ = 0;
  width_ = 0;
  collision_data_ = nullptr;
}

Map::~Map()
{
  free_resources();
}

Float2 Map::ratio() const
{
  return ratio_;
}

ESAT::SpriteHandle Map::background() const
{
  return background_;
}


bool Map::isOccupied(const s32 x, const s32 y) const
{
  if (!isValidPosition(x, y)) return true;
  const s32 position = x + (y * width_);
  return !collision_data_[position];
}

void Map::free_resources()
{
  if (collision_data_) free(collision_data_);
  ESAT::SpriteRelease(background_);
}

s16 Map::loadMap(const char* src, const char* original)
{
  //If there's already data loaded we free it
  if (collision_data_) free_resources();

  s32 bpp;

  unsigned char* background_image = stbi_load(original, &original_width_, &original_height_, &bpp, 1);

  if (!background_image) return kErrorCode_Memory;

  unsigned char* image_data = stbi_load(src, &width_, &height_, &bpp, 1);

  if (!image_data) {
    stbi_image_free(background_image);
    return kErrorCode_Memory;
  }

  const int number_of_elements = height_*width_;
  collision_data_ = static_cast<bool*>(malloc(sizeof(bool)*number_of_elements));

  if (!collision_data_) {
    stbi_image_free(background_image);
    stbi_image_free(image_data);
    return kErrorCode_Memory;
  }

  for (int i = 0; i < width_*height_; i++)
  {
    const bool result = image_data[i] == 0xff;
    collision_data_[i] = result;
  }
  ratio_ = Float2(static_cast<float>(original_width_) / width_, static_cast<float>(original_height_) / height_);

  background_ = ESAT::SpriteFromFile(original);

  stbi_image_free(background_image);
  stbi_image_free(image_data);

  return kErrorCode_Ok;
}

bool Map::isValidPosition(const s32 x, const s32 y) const
{
  if (x > width_ || x < 0) return false;
  if (y > height_ || y < 0) return false;
  return true;
}
