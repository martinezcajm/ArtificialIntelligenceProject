#include "path_finder.h"
#include "STB/stb_image.h"
#include "path.h"
#include "common_def.h"
#include "map.h"
#include "gamestate.h"

PathFinder::PathFinder(Map& map)
{
  //current_map_ = map;
  a_star_ = new AStar();
}

PathFinder::~PathFinder()
{
  delete(a_star_);
}


s16 PathFinder::GeneratePath(/*origin, dest, */ Path* path, Float2 origin, Float2 dst)
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
   *  Another long and hard
   *  a_star_->generatePath(Float2{ 50.0f * 8,40.0f * 8 }, Float2{ 80.0f,40.0f * 8 }, path, GameState::instance().map_);
   *  
   */
  a_star_->generatePath(origin, dst, path, GameState::instance().map_);
  
  

  return kErrorCode_Ok;
}

void PathFinder::update(const uint32_t dt)
{
  
}
