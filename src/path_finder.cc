#include "path_finder.h"
#include "STB/stb_image.h"
#include "path.h"
#include "common_def.h"

PathFinder::PathFinder()
{
  current_map_.height_ = 0;
  current_map_.width_ = 0;
  current_map_.collision_data_ = nullptr;
  //calculating_path_ = false; 
  a_star_ = new AStar();
}

PathFinder::~PathFinder()
{
  if (current_map_.collision_data_) free(current_map_.collision_data_);
  delete(a_star_);
}


s16 PathFinder::LoadMap(const char* src, const float original_map_width, const float original_map_height)
{

  s32 bpp;
  unsigned char* image_data = stbi_load(src, &current_map_.width_, &current_map_.height_, &bpp, 1);

  if (!image_data) return kErrorCode_Memory;
  if (current_map_.collision_data_) free(current_map_.collision_data_);

  const int number_of_elements = current_map_.height_*current_map_.width_;
  current_map_.collision_data_ = static_cast<bool*>(malloc(sizeof(bool)*number_of_elements));

  if (!current_map_.collision_data_) return kErrorCode_Memory;

  for(int i = 0; i < current_map_.width_*current_map_.height_; i++)
  {
    const bool result = image_data[i] == 0xff;
    current_map_.collision_data_[i] = result;
  }
  current_map_.ratio_ = Float2(original_map_width/current_map_.width_, original_map_height/current_map_.height_);
  return kErrorCode_Ok;
}

s16 PathFinder::GeneratePath(/*origin, dest, */ Path* path)
{
  /*
   * TESTS
   *  Not found (trying to go through water
   *  a_star_->generatePath(Float2{ 0.0f,0.0f }, Float2{ 956.0f, 0.0f }, path, current_map_);
   *  Found (bording the river)
   *  a_star_->generatePath(Float2{ 0.0f,0.0f }, Float2{ 937.0f,230.0f }, path, current_map_);
   *  Found (straight line)
   *  a_star_->generatePath(Float2{416.0f,32.0f}, Float2{640.0f,32.0f}, path, current_map_);
   *  Found (obstacle)
   *  a_star_->generatePath(Float2{ 416.0f,32.0f }, Float2{ 792.0f,32.0f }, path, current_map_);
   *  Found (long and hard)
   *  a_star_->generatePath(Float2{ 0.0f,0.0f }, Float2{ 374.0f,448.0f }, path, current_map_);
   *  
   */
  a_star_->generatePath(Float2{ 0.0f,0.0f }, Float2{ 374.0f,448.0f }, path, current_map_);

  return kErrorCode_Ok;
}

//void PathFinder::update(const uint32_t dt)
//{
//  
//}
