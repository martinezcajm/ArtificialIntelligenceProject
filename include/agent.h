// agent.h
// Jose Maria Martinez
// Header of the functions of the agent class
#ifndef __AGENT_H__
#define __AGENT_H__
#include <cstdint>
#include <float2.h>

enum class MovementType
{
  k_MovDeterminist = 0,
  k_MovRandom = 1,
  k_MovTracking = 2,
  k_MovPattern = 3,
  k_MovStop = 4,
  k_PADDING = 255
};

enum class AgentType
{
  k_Patrol = 0,
  k_Scout = 1,
  k_Chaser = 2,
  k_Mindless = 3,
  k_PADDING = 255
};

enum class PatternToken
{
  k_East = 0,
  k_West = 1,
  k_South = 2,
  k_North = 3,
  k_PADDING = 255
};

enum class FSMStates
{
  k_Init = 0,
  k_Home = 1,
  k_Wander = 2,
  k_Chase = 3,
  k_Eating = 4,
  k_Flee = 5,
  k_Dead = 6,
  k_GoHome = 7,
  k_End = 8,
  k_PADDING = 255
};

enum class FSMGoHomeStates
{
  k_Init = 0,
  k_CalculatePath = 1,
  k_Waiting = 2,
  k_End = 3,
  k_PADDING = 255
};

struct PatternCommand
{
  PatternToken token;
  uint32_t seconds;
};

/** @brief Agent entity
*
* This class is the representation of an agent that can
* move and think
*
*/
class Agent
{
public:
  /** @brief Agent constructor
  *
  * Default agent constructor, initializes the agent as a 
  * mindless one at the point 0,0 
  *
  * @return *Agent
  */
  Agent();

  /** @brief Agent constructor
  *
  * Agent constructor. Depending of the rol assigned will have a
  * different movement type:
  *  -Patrol:   will follow a pattern
  *  -Scout:    will do a random movement 
  *  -Chaser:   will track 
  *  -Mindless: will follow a determinist movement
  *
  * @return *Agent
  * @param agent_type role of the agent
  * @param x start x coordinate of the agent
  * @param y start y coordinate of the agent
  */
  Agent(const AgentType agent_type, const float x, const float y);
  /** @brief Destroys the Agent 
  *
  * Destructor of the agent entity
  *
  * @return void
  */
  ~Agent();
  /** @brief Updates the agent
  *
  * Updates the body and mind of the agent based on a delta time
  *
  * @param dt time that has passed in the game world
  * @return void
  */
  void update(const uint32_t dt);
  /** @brief gets the x position of the agent
  *
  * Returns the x position of the agent.
  *
  * @return float x position
  */
  float x() const;
  /** @brief gets the y position of the agent
  *
  * Returns the y position of the agent.
  *
  * @return float y position
  */
  float y() const;
private:
  uint32_t id_ = 0;
  

  //The epsilon will vary depending on the speed * kEpsilonFactor
  float epsilon_;
  const float kEpsilonFactor = 0.01f;
  
  MovementType move_type_;
  PatternToken actual_pattern_;
  AgentType type_agent_;
  //bool target_reached_;

  
  //Agent* target;
  Float2 position_;
  Float2 target_position_;
  Float2 velocity_;

  //float vision_range_ = 200;

  //speed as m/s 
  float speed_;
  const float kSpeedUp = 2.0f;

  bool initialized_;

  //Determinist variables
  int determinist_idx_;
  static const int determinist_size_ = 2;
  Float2 determinist_targets_[determinist_size_];
  //Pattern variables
  int pattern_idx_;
  static const int pattern_size_ = 4;
  PatternCommand pattern_targets_[pattern_size_];
  uint32_t accum_time_pattern_;
  float pattern_step_;
  //Tracking variables
  uint32_t tracking_retarget_time_;
  uint32_t accum_time_tracking_;
  //Random variables
  uint32_t next_random_time_;
  uint32_t accum_time_random_;

  //FSM variables
  FSMStates actual_state_;
  int32_t timer_;
  const int32_t exist_home_time_ = 3000;
  const int32_t lose_focus_time_ = 5000;
  const int32_t invencibility_duration_ = 8000;
  const int32_t death_time = 2000;
  bool pill_grabbed_;
  //TODO this will be the variables of the agent, for now we have 
  //TODO specific ones for testing the FSM
  const float fsm_epsilon_ = 50.0f;
  float fsm_distance_;
  const float track_distance_ = 20.0f;

  FSMGoHomeStates actual_state_go_home_;

  /** @brief Initializes the agent
  *
  * Initializes the attributes from the agent.
  * This, method is called on the constructor of the class.
  *
  * @param x x coordinate at which the agent will be started
  * @param y y coordinate at which the agent will be started
  * @return void
  */
  void init(const float x, const float y);

  /** @brief Updates the mind of the agent
  *
  * Method in charge of the decision making of the agent.
  * Switches between movements and sets the orders for the body.
  *
  * @param dt time that has passed in the game world
  * @return void
  */
  void updateMind(const uint32_t dt);
  /** @brief Updates the body of the agent
  *
  * Updates the physic representation of the agent. Deals with
  * the movement and updates the sensors.
  *
  * @param dt time that has passed in the game world
  * @return void
  */
  void updateBody(const uint32_t dt);
  /** @brief Determinist behaviour
  *
  * Realizes the determinist movement. Advances through the path established
  * by the mind. The path that follow is absolute.
  *
  * @param dt time that has passed in the game world
  * @return void
  */
  void MOV_Determinist(const uint32_t dt);
  /** @brief Random behaviour
  *
  * Realizes the random movement. It's in charge of selecting
  * the next random position in case the threshold requirement has
  * been reached.
  *
  * @param dt time that has passed in the game world
  * @return void
  */
  void MOV_Random(const uint32_t dt);
  /** @brief Tracking behaviour
  *
  * Realizes the tracking movement. Makes the body advance to the 
  * target position that the mind has established.
  *
  * @param dt time that has passed in the game world
  * @return void
  */
  void MOV_Tracking(const uint32_t dt);
  /** @brief Pattern behaviour
  *
  * Realizes the pattern movement. Advances through the path established
  * by the mind. he path that follow is relative to the agent.
  *
  * @param dt time that has passed in the game world
  * @return void
  */
  void MOV_Pattern(const uint32_t dt);
  /** @brief Stop behaviour
  *
  * Stops the movement.
  *
  * @param dt time that has passed in the game world
  * @return void
  */
  void MOV_Stop(const uint32_t dt);
  /** @brief moves the agent
  *
  * Moves the agent based on the velocity it has at the moment. The agent
  * will be moved using the e = v * t equation. Being v the velocity_ of the agent
  * and t the delta time elapsed at the game world in seconds.
  *
  * @param dt time that has passed in the game world
  * @return void
  */
  void move(const uint32_t dt);
  /** @brief Sets the orientation of the agent
  *
  * Calculates the normalized velocity_ we need to reach our destination
  *
  * @param destination to which we are going to move
  * @return void
  */
  void setOrientation(const Float2& destination);
  /** @brief Calculates our velocity
  *
  *  Sets the orientation of the agent and escalates the resultant velocity_
  *  based on the speed_ of the agent.
  *
  * @return void
  */
  void calculateVelocity();
  /** @brief Checks if the position has been reached
  *
  * Checks whether or not the agent has reached it's destination.
  *
  * @return bool true if the position has been reached
  */
  bool positionReached() const;  
  /** @brief sets the next position of the agent
  *
  * Updates the destination the agent needs to reach.
  *
  * @param new_target_x x coordinate of the new_position
  * @param new_target_y y coordinate of the new_position
  * @return void
  */
  void setNextPosition(float new_target_x, float new_target_y);

  bool goHomeFSM();

  //bool isPlayerAtSight() const;
};


#endif
