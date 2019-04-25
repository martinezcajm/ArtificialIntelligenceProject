#include "path_finder.h"
#include "STB/stb_image.h"
#include "path.h"

PathFinder::PathFinder()
{
  current_map_.height = 0;
  current_map_.width = 0;
  current_map_.collisionData = nullptr;
}

PathFinder::~PathFinder()
{
  if (current_map_.collisionData) free(current_map_.collisionData);
}


s16 PathFinder::LoadMap(const char* src)
{
  s32 bpp;
  unsigned char* image_data = stbi_load(src, &current_map_.width, &current_map_.height, &bpp, 1);

  if (!image_data) return kErrorCode_Memory;
  if (current_map_.collisionData) free(current_map_.collisionData);

  const int number_of_elements = current_map_.height*current_map_.width;
  current_map_.collisionData = static_cast<bool*>(malloc(sizeof(bool)*number_of_elements));

  if (!current_map_.collisionData) return kErrorCode_Memory;

  for(int i = 0; i < current_map_.width*current_map_.height; i++)
  {
    const bool result = image_data[i] == 0xff;
    current_map_.collisionData[i] = result;
  }
  return kErrorCode_Ok;
}

s16 PathFinder::GeneratePath(Path* path/*origin, dest, *path*/)
{
  if (!path) return kErrorCode_InvalidPointer;
  path->clear();
  path->create(20);
  path->addPoint(0.0f, 0.0f, 0.0f);
  path->addPoint(5.0f, 5.0f, 0.0f);
  path->setToReady();
  return kErrorCode_Ok;
}

