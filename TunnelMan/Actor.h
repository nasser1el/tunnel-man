#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// ACTOR

class Actor : public GraphObject
{
public:

	Actor(int imageID, int x, int y, StudentWorld* studentWorld, Direction direction, float size, unsigned int depth);

	virtual ~Actor();

	virtual void doSomething() = 0;

	// NAME: annoyed
	// INPUT: 1 int
	// OUTPUT: NONE
	// DESCRIPTION: Used only for the tunnelman and protestors but implemented as
	// a virtual function in actors so I could use it in the StudentWorld class
	virtual void annoyed(int amount);

	// NAME: bribes
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: Used only for protestors but implemented as
	// a virtual function in actors so I could use it in the StudentWorld class
	virtual void bribed();

	// mutators

	// NAME: kill
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: Sets the life status of the current actor to false (dead) so
	// that it doesn't perform any of its actions during the ticks
	void kill();

	// NAME: setProtestor
	// INPUT: 1 int 
	// OUTPUT: NONE
	// DESCRIPTION: The actor class has a private variable m_protestor that I 
	// used to keep track of which actors were protestors. By default, this variable
	// was set to 0 (not a protestor) but the RegularProtestor class sets it to 1 and
	// the HardcoreProtestor class sets it to 2
	void setProtestor(int status);

	// NAME: setHitPoints
	// INPUT: 1 int
	// OUTPUT: NONE
	// DESCRIPTION: sets an actor's hitpoints (health) to the input
	void setHitPoints(int amount);

	// NAME: setLifeStatus
	// INPUT: 1 bool
	// OUTPUT: NONE
	// DESCRIPTION: sets an actor's life status to the input (true/false)
	void setLifeStatus(bool status);

	// accessors
	
	// NAME: getWorld
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: returns a pointer to the current StudentWorld (allows
	// me to use StudentWorld functions like getLevel() or getTunnelMan())
	StudentWorld* getWorld();

	// NAME: getLifeStatus
	// INPUT: NONE
	// OUTPUT: 1 bool
	// DESCRIPTION: returns an actor's life status
	bool getLifeStatus();

	// NAME: getHitPoints
	// INPUT: NONE
	// OUTPUT: 1 int
	// DESCRIPTION: returns an actor's hitpoints (health)
	int getHitPoints();

	// NAME: getProtestor
	// INPUT: NONE
	// OUTPUT: 1 int
	// DESCRIPTION: returns an actor's m_protestor value (tells us whether 
	// the actor is not a protestor, a regular protestor, or a hardcore protestor)
	int getProtestor();

private:
	StudentWorld* studentWorldPointer;
	bool m_lifeStatus;
	int m_hitPoints;
	// 0 = not a protestor
	// 1 = regular protestor
	// 2 = hardcore protestor 
	int m_protestor;
};

// EARTH

class Earth : public Actor
{
public:
	Earth(int x, int y, StudentWorld* studentWorld);

	virtual ~Earth();

	// NAME: doSomething 
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: It’s earth–what do you expect it to do? It does nothing! As such, it doesn't need to have a doSomething() method... 
	// Or if it does have one, it doesn't need to do anything. (from specs)
	virtual void doSomething();
};

// TUNNELMAN

class TunnelMan : public Actor
{
public:
	TunnelMan(StudentWorld* studentWorld);

	virtual ~TunnelMan();

	// NAME: doSomething
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: When given an opportunity to do something, the TunnelMan must do the following: 
	// 1. The TunnelMan must check to see if it is currently alive. If not, then its doSomething() method must return immediately –none of the following steps should be performed.
	// 2. If the TunnelMan’s 4x4 image currently overlaps any Earth objects within the oil field, then it will call upon the StudentWorld object to:
		// a. Remove/destroy the Earth objects from the 4x4 area occupied by the TunnelMan (from x, y to x+3,y+3 inclusive) 
		// b. Make a digging sound by playing the SOUND_DIG sound (see the StudentWorld section of this document for details on how to play a sound).
	// 3. Otherwise,the doSomething() method must check to see if the human player pressed a key (the section below shows how to check if the user pressed a key, and if so, how to determine what key the user pressed).
		// a. If the user presses the Escape key, this allows the user to abort the current level. In this case, the TunnelMan object should set itself to completely annoyed (aka dead).
		// The code in your StudentWorld class should detect that the TunnelMan has died and address this appropriately (e.g., replay the level from scratch, or end the game if the player has run out of lives).
		// b. If the user presses the spacebar key, then the TunnelMan will fire a Squirt into the oil field, assuming they have sufficient water in their squirt gun to do so.
		// The TunnelMan will then reduce their water count by 1. To fire a Squirt, the player must create and add a new Squirt object into the oil field at a location that is four squares immediately in front of 
		// the TunnelMan, facing the same direction as the TunnelMan. So if the TunnelMan is at x = 10, y = 7, and he is facing upward, then the new Squirt object would be placed at location x = 10, y = 11, and the Squirt would be facing upward.
		// Every time the TunnelMan fires a Squirt, it must play the sound SOUND_PLAYER_SQUIRT. If the initial location where the Squirt starts (i.e., 4 squares in front of the TunnelMan) is occupied by one or more Earth objects or 
		// is within a radius of 3.0 of a Boulder object, then the TunnelMan must still make a squirting noise and will waste 1 unit of water, but the TunnelMan must not add any Squirt object into the oil field and thus no 
		// damage can be done to any Protesters (it’s as if the TunnelMan shot water against a wall and it immediately dissipated). 
		// Hint: One way to implement this is to have your TunnelMan create a new Squirt object in the proper location and facing the proper direction, and then give it to your StudentWorld to manage(i.e., to animate) along with your other game objects.
		// c. If the player wants their TunnelMan to turn to face a different direction then it is currently facing(by pressing one of the arrow keys), then the TunnelMan’s direction should be 
		// adjusted to the indicated direction but the TunnelManmust not move in that direction. For example, if theTunnelMan is currently facing up, and the player hits the right arrow key, then the TunnelMan
		// should turn to face right, but not move right.
		// d. Else, if the TunnelMan is already facing in a particular direction and the user hits the arrow key to move in that same direction, AND the TunnelMan is allowed to move in that direction(e.g., there is no Boulder in 
		// the way, and the position doesn’t take the TunnelMan outside of the oilfield), then your code must use the GraphObject’s moveTo() method to adjust the TunnelMan’s location by one square in the indicated direction.
		// Valid coordinates for the TunnelMan are from x = 0, y = 0 to x = 60, y = 60, inclusive. Moreover, the TunnelMan cannot occupy a square that is less than or equal to a radius of 3 away from the center of any Boulder.
		// e. If the player presses the ‘Z’ or ‘z’ keys and the TunnelMan has one or more sonar charges remaining in his inventory, then this will cause the TunnelMan to use his sonar charge to illuminate the contents of the oil field
		// within a radius of 12 of his location: 
			// i. The count of sonar charges held by the TunnelMan must be decremented by 1.
			// ii. All hidden game objects (e.g., Gold Nuggets or Barrels of oil) that are within a radius of 12 (e.g., this includes 11.99 squares away) must be made visible via setVisible() and revealed to the player.
		// f. If the player presses the TAB key and they have one or more units of gold in their inventory, then they must add a Gold Nugget object into the oil field at their current x, y location and reduce their gold count by 1 unit. 
		// The dropped Gold Nugget must have a temporary state and a lifetime of 100 game ticks, and is only pickup-able by Protesters (either Regular or Hardcore). Such a dropped Nugget will start out in a visible state
	// When the TunnelMan is annoyed (i.e., shouted at by a Protester), its hit-points should be decremented by the appropriate amount (e.g., 2 points). 
	// Hint: All objects that can be annoyed should have some type of method that can be called to annoy them. If the TunnelMan’s hit-points reach zero or below due to being annoyed, the TunnelMan must set its object’s state to dead, and 
	// then play an “I give up” sound effect: SOUND_PLAYER_GIVE_UP. 
	// Note: The StudentWorld class should check the TunnelMan’s status during each tick and if he transitions to a dead state, it should return the appropriate value indicating that the player lost a life.
	// pg. 27/28/29
	virtual void doSomething();

	// NAME: annoyed
	// INPUT: 1 int
	// OUTPUT: NONE
	// DESCRIPTION: Reduces the tunnelman's hitpoints (health) by the amount passed
	// in by the function (however much the tunnelman was annoyed by) and then checks
	// if the tunnelman's hitpoints have fallen to or below 0 and, if so, sets his life
	// status to dead (false)
	virtual void annoyed(int amount);

	// mutators

	// NAME: setSquirts
	// INPUT: 1 int
	// OUTPUT: NONE
	// DESCRIPTION: Sets the tunnelman's squirts to the amount passed
	// in by the function
	void setSquirts(int amount);

	// NAME: setGold
	// INPUT: 1 int
	// OUTPUT: NONE
	// DESCRIPTION: Sets the tunnelman's gold count to the amount passed
	// in by the function
	void setGold(int amount);

	// NAME: setSonarCharges
	// INPUT: 1 int
	// OUTPUT: NONE
	// DESCRIPTION: Sets the tunnelman's sonar charge count to the amount passed
	// in by the function
	void setSonarCharges(int amount);

	// NAME: setBarrels
	// INPUT: 1 int
	// OUTPUT: NONE
	// DESCRIPTION: Sets the tunnelman's barrel count to the amount passed
	// in by the function
	void setBarrels(int amount);

	// accessors

	// NAME: getSquirts
	// INPUT: NONE
	// OUTPUT: 1 int
	// DESCRIPTION: returns the tunnelman's squirt count
	int getSquirtsLeftInSquirtGun();

	// NAME: getPlayerGoldCount
	// INPUT: NONE
	// OUTPUT: 1 int
	// DESCRIPTION: returns the tunnelman's gold count
	int getPlayerGoldCount();

	// NAME: getPlayerSonarChargeCount
	// INPUT: NONE
	// OUTPUT: 1 int
	// DESCRIPTION: returns the tunnelman's sonar charge count
	int getPlayerSonarChargeCount();

	// NAME: getNumberOfBarrels
	// INPUT: NONE
	// OUTPUT: 1 int
	// DESCRIPTION: returns the tunnelman's barrel count
	int getNumberOfBarrels();

	// NAME: getNumberOfBarrelsRemainingToBePickedUp()
	// INPUT: NONE
	// OUTPUT: 1 int
	// DESCRIPTION: returns the number of barrels the tunnelman
	// still needs to pick up to end the level
	int getNumberOfBarrelsRemainingToBePickedUp();

private:
	int m_squirts;
	int m_sonarCharges;
	int m_goldNuggets;
	int m_barrelsOfOil;
	int m_barrelsOfOilToPickUp;
};

// BOULDER

class Boulder : public Actor
{
public:
	Boulder(int x, int y, StudentWorld* studentWorld);

	// NAME: doSomething
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: Each time the Boulder object is asked to do something (during a tick), it should:
	// 1. Check to see if it’s still alive. If not, its doSomething() method should immediately return.
	// 2. If the Boulder is currently in the stable state, then it must check to see if there is any Earth in the 4 squares immediately below it. For example, if the Boulder is at location x = 20, y = 30, it 
	// would check squares (20, 29), (21, 29), (22, 29) and (23, 29) for Earth. If there is any Earth below the Boulder, it does nothing –after all it’s a dumb rock. 
	// However, if none of the 4 squares beneath the Boulder have any Earth, then the Boulder must transition into a waiting state: 
		// a. It must enter a waiting state for the next 30 ticks 
	// 3. If the Boulder is in a waiting state and 30 ticks have elapsed, then it must transition into a falling state and play the sound SOUND_FALLING_ROCK.
	// 4. If the Boulder is in a falling state, then: 
		// a. It must continue to move downward one square during each tick until it either: 
			// (a) hits the bottom of the oil field (i.e., it tries to move to y = -1),  
			// (b) runs into the top of another Boulder, or 
			// (c) it runs into Earth (i.e., by moving down a square, the Boulder would overlap over one or more Earth objects).
			// When any of the above conditions are met the Boulder must set its state to dead so it can be removed from the game at the end of the current tick.
		// b. If the Boulder comes within a radius of 3 (i.e. less than or equal to 3 squares, so 2.99 would count) of any Protester(s) or the TunnelMan while falling, it must 
		// cause 100 points of annoyance to those actors (effectively completely annoying them in one fell swoop). The Boulder will continue falling down as described above, even 
		// if it annoys one or more Protesters on its way down (i.e., it won’t stop falling if it runs into a Protester). However, if the Boulder annoys the TunnelMan, the player will instantly lose a life.
	// pg. 32
	virtual void doSomething();

	// mutators

	// NAME: setStateToStable
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: sets m_state (used to track the boulder's state)
	// to 1 (stable)
	void setStateToStable();

	// NAME: setStateToWaiting
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: sets m_state (used to track the boulder's state)
	// to 2 (waiting)
	void setStateToWaiting();

	// NAME: setStateToFalling
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: sets m_state (used to track the boulder's state)
	// to 3 (falling)
	void setStateToFalling();

private:
	// 1 = stable
	// 2 = waiting
	// 3 = falling
	int m_state;
	int m_ticks;
};

// SQUIRT

class Squirt : public Actor
{
public:
	Squirt(int x, int y, Direction direction, StudentWorld* studentWorld);

	// NAME: doSomething
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: Each time the Squirt object is asked to do something (during a tick):
	// 1. If a Squirt is within a radius of 3.0 of one or more Protesters (up to and including a distance of 3.0 squares away), it will cause 2 points
	// of annoyance to these Protester(s), and then immediately set its state to dead, so it can be removed from the oil field at the end of the tick. 
	// 2. If a Squirt has traveled through its full travel distance, then it immediately sets its state to dead, so it can be removed from the oil field
	// at the end of the current tick.
	// 3. Otherwise, the Squirt must check to see if it can move one square in its currently-facing direction. If this target location is occupied by either Earth
	// or a Boulder, then the Squirt immediately sets its state to dead so it can be removed from the oil field at the end of the current tick.
	// 4. Otherwise, the Squirt moves one square forward in its currently-facing direction, and then returns.
	// pg. 33
	virtual void doSomething();

private:
	int m_travelDistance;
	int m_maxTravelDistance; 
};

// GOODIE

class Goodie : public Actor
{
public:
	Goodie(int imageID, int x, int y, Direction direction, StudentWorld* studentWorld, float size, unsigned int depth);

	// mutators

	// NAME: setTicks
	// INPUT: 1 int
	// OUTPUT: NONE
	// DESCRIPTION: sets m_ticks to the value passed in by
	// the function
	void setTicks(int numberOfTicks);

	// accessors

	// NAME: getTicks
	// INPUT: NONE
	// OUTPUT: 1 int
	// DESCRIPTION: returns the number of ticks 
	int getTicks();

private:
	// I added m_ticks to the Goodie base class since I realized all Goodies used
	// ticks so I just added getTicks and setTicks to the function as well to be able
	// to access and change the m_ticks value for each Goodie
	int m_ticks;
};

// BARRELOFOIL

class BarrelOfOil : public Goodie
{
public:
	BarrelOfOil(int x, int y, StudentWorld* studentWorld);

	// NAME: doSomething
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: Each time the Barrel object is asked to do something (during a tick):
	// 1. The object must check to see if it is currently alive. If not, then its doSomething() method must return immediately –none of the following steps should be performed.
	// 2. Otherwise, if the Barrel is not currently visible AND the TunnelMan is within a radius of 4.0 of it (<= 4.00 units away), then:
		// a. The Barrelmust make itself visible with the setVisible() method.
		// b. The Barrel’s doSomething() method must immediately return.
	// 3. Otherwise, if the Barrel is within a radius of 3.0 (<= 3.00 units away) from the TunnelMan, then the Barrel will activate, and:
		// a. The Barrel must set its state to dead (so that it will be removed by your StudentWorld class from the game at the end of the current tick).
		// b. The Barrel must play a sound effect to indicate that the player picked up the Goodie: SOUND_FOUND_OIL.
		// c. The Barrel must increase the player’s score by 1000 points.
		// d. If necessary, the Barrel may inform the StudentWorld object that it was picked up. Once all Barrels 
		// on the level have been picked up, the player finishes the level and may advance to the next level.
	// pg. 34
	virtual void doSomething(); 
};

// GOLDNUGGET

class GoldNugget : public Goodie
{
public:
	GoldNugget(int x, int y, StudentWorld* studentWorld, int state);

	// NAME: doSomething
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: Each time the Gold Nugget object is asked to do something (during a tick):
	// 1. The object must check to see if it is currently alive. If not, then its doSomething() method must return immediately –none of the following steps should be performed.
	// 2. Otherwise, if the Gold Nugget is not currently visible AND the TunnelMan is within a radius of 4.0 of it (<= 4.00 units away), then:
		// a. The Gold Nugget must make itself visible with the setVisible()method.
		// b. The Gold Nugget doSomething() method must immediately return.
	// 3. Otherwise, if the Gold Nugget is pickup-able by the TunnelMan and it is within a radius of 3.0 (<= 3.00 units away) from the TunnelMan, then the Gold Nugget will activate, and:
		// a. The Gold Nugget must set its state to dead (so that it will be removed by your StudentWorld class from the game at the end of the current tick).
		// b. The Gold Nugget must play a sound effect to indicate that the TunnelMan picked up the Goodie: SOUND_GOT_GOODIE.
		// c. The Gold Nugget increases the player’s score by 10 points (This increase can be performed by the TunnelMan class or the Gold Nugget class).
		// d. The Gold Nugget must tell the TunnelMan object that it just received a new Nugget so it can update its inventory.
	// 4. Otherwise, if the Gold Nugget is pickup-able by Protesters and it is within a radius of 3.0 (<= 3.00 units away) from a Protester, then the Gold Nugget will activate, and: 
		// a. The Gold Nugget must set its state to dead (so that it will be removed by your StudentWorld class from the game at the end of the current tick).
		// b. The Gold Nugget must play a sound effect to indicate that a Protestor picked it up: SOUND_PROTESTER_FOUND_GOLD.
		// c. The Gold Nugget must tell the Protester object that it just received a new Nugget so it can react appropriately (e.g., be bribed).
		// d. The Gold Nugget increases the player’s score by 25 points (This increase can be performed by the Protester class or the Gold Nugget class).
		// Note: A Gold Nugget can only bribe a single Protester (either Regular or Hardcore) before disappearing from the game. If multiple Protesters are within the activating radius of 
		// the Nugget, then only one of the Protesters must be bribed.
	// 5. If the Gold Nugget has a temporary state, then it will check to see if its tick lifetime has elapsed, and if so it must set its state to dead(so that it will be removed by your 
	// StudentWorld class from the game at the end of the current tick).
	// pg. 35/36
	virtual void doSomething();

	// mutators

	// NAME: setState
	// INPUT: 1 int 
	// OUTPUT: NONE
	// DESCRIPTION: sets m_state (used to keep track of the gold nugget's state) to
	// the input (0 = pickup-able-by-the-tunnelman, 1 = pickup-able-by-protestors) 
	void setState(int state);

private:
	// 0 = permanent (pickup-able-by-the-tunnelman)
	// 1 = temporary (pickup-able-by-protestors)
	int m_state;
};

// SONARKIT

class SonarKit : public Goodie
{
public:
	SonarKit(int x, int y, StudentWorld* studentWorld);

	// NAME: doSomething
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: Each time the Sonar Kit object is asked to do something (during a tick): 
	// 1. The object must check to see if it is currently alive. If not, then its doSomething() method must return immediately –none of the following steps should be performed.
	// 2. Otherwise, if the Sonar Kit is within a radius of 3.0 (<= 3.00 units away) from the TunnelMan, then the Sonar Kit will activate, and: 
		// a. The Sonar Kit must set its state to dead (so that it will be removed by your StudentWorld class from the game at the end of the current tick).
		// b. The Sonar Kit must play a sound effect to indicate that the TunnelMan picked up the Goodie: SOUND_GOT_GOODIE.
		// c. The Sonar Kit must tell the TunnelMan object that it just received a new SonarKit so it can update its inventory.
		// d. The Sonar Kit increases the player’s score by 75 points (This increase can be performed by the TunnelMan class or the Sonar Kit class).
	// 3. Since the Sonar Kit is always in a temporary state, it will check to see if its tick lifetime has elapsed, and if so it must set its state 
	// to dead (so that it will be removed by your StudentWorld class from the game at the end of the current tick).
	// pg. 37
	virtual void doSomething();

};

// WATERPOOL

class WaterPool : public Goodie
{
public:
	WaterPool(int x, int y, StudentWorld* studentWorld);

	// NAME: doSomething
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: Each time a Water pool object is asked to do something (during a tick):
	// 1. The object must check to see if it is currently alive. If not, then its doSomething() method must return immediately –none of the following steps should be performed.
	// 2. Otherwise, if the Waterpool is within a radius of 3.0 (<= 3.00 units away) from the TunnelMan, then the Water pool will activate, and:
		// a. The Waterpool must set its state to dead (so that it will be removed by your StudentWorld class from the game at the end of the current tick).
		// b. The Waterpool must play a sound effect to indicate that the TunnelMan picked up the Goodie: SOUND_GOT_GOODIE.
		// c. The Waterpool must tell the TunnelMan object that it just received 5 new squirts of water so it can update its inventory.
		// d. The Waterpool increases the player’s score by 100 points (This increase can be performed by the TunnelMan class or the Water Pool class). 
	// 3. Since the Pool of Water is always in a temporary state, it will check to see if its tick lifetime has elapsed, and if so it must set its state 
	// to dead(so that it will be removed by your StudentWorldclass from the game at the end of the current tick).
	// pg. 38
	virtual void doSomething();

};

// PROTESTOR

// Code for Point from Lecture 5 slides
class Point
{
public:
	Point(int x, int y);
	int getx() const;
	int gety() const;
private:
	int m_x, m_y;
};

class Protestor : public Actor
{
public:
	Protestor(int x, int y, StudentWorld* studentWorld, int imageID);

	// NAME: annoyed
	// INPUT: 1 int
	// OUTPUT: NONE
	// DESCRIPTION: Reduces the protestor's hitpoints (health) by the amount passed
	// in by the function (however much the protestor was annoyed by) and then checks
	// if the protestor's hitpoints have fallen to or below 0 and, if so, sets their
	// state to leave the oil field (true)
	virtual void annoyed(int amount);

	// NAME: setNumSquaresToMoveInCurrentDirection
	// INPUT: NONE
	// OUTPUT: NONE 
	// DESCRIPTION: sets m_numSquaresToMove to a random number between 8 and 60
	void setNumSquaresToMoveInCurrentDirection();

	// NAME: bribed
	// INPUT: NONE
	// OUTPUT: NONE 
	// DESCRIPTION: puts the protestor into a leave-the-oil-field-state and
	// plays the correct sound
	virtual void bribed();

	// accesors

	// NAME: getNumSquaresToMoveInCurrentDirection
	// INPUT: NONE
	// OUTPUT: 1 int
	// DESCRIPTION: returns m_numSquaresToMove
	int getNumSquaresToMoveInCurrentDirection();

	// NAME: getTicksToWaitBetweenMoves
	// INPUT: NONE
	// OUTPUT: 1 int
	// DESCRIPTION: returns m_ticksToWaitBetweenMoves
	int getTicksToWaitBetweenMoves();

	// NAME: getTicks
	// INPUT: NONE
	// OUTPUT: 1 int
	// DESCRIPTION: returns m_ticks
	int getTicks();

	// NAME: getNonRestingTicks
	// INPUT: NONE
	// OUTPUT: 1 int
	// DESCRIPTION: returns m_nonRestingTicks
	int getNonRestingTicks();

	// NAME: getNonRestingPerpindicularTicks
	// INPUT: NONE
	// OUTPUT: 1 int
	// DESCRIPTION: returns m_nonRestingPerpindicularTicks
	int getNonRestingPerpindicularTicks();

	// NAME: getLeaveTheOilFieldState
	// INPUT: NONE
	// OUTPUT: 1 bool
	// DESCRIPTION: returns m_leaveTheOilFieldState
	bool getLeaveTheOilFieldState();

	// mutators 

	// NAME: setNumSquaresToMoveInCurrentDirection
	// INPUT: 1 int 
	// OUTPUT: NONE
	// DESCRIPTION: sets m_numSquaresToMove to the input
	void setNumSquaresToMoveInCurrentDirection(int amount);

	// NAME: setTicks
	// INPUT: 1 int 
	// OUTPUT: NONE
	// DESCRIPTION: sets m_ticks to the input
	void setTicks(int amount);

	// NAME: setNonRestingTicks
	// INPUT: 1 int 
	// OUTPUT: NONE
	// DESCRIPTION: sets m_nonRestingTicks to the input
	void setNonRestingTicks(int amount);

	// NAME: setNonRestingPerpindicularTicks
	// INPUT: 1 int 
	// OUTPUT: NONE
	// DESCRIPTION: sets m_nonRestingPerpindicularTicks to the input
	void setNonRestingPerpindicularTicks(int amount);

	// NAME: setTicksToWaitBetweenMoves
	// INPUT: 1 int 
	// OUTPUT: NONE
	// DESCRIPTION: sets m_ticksToWaitBetweenMoves to the input
	void setTicksToWaitBetweenMoves(int amount);

	// NAME: setLeaveTheOilFieldState
	// INPUT: 1 bool
	// OUTPUT: NONE
	// DESCRIPTION: sets m_leaveTheOilFieldState to the input (true/false)
	void setLeaveTheOilFieldState(bool state);

	// NAME: protestorQueueMazeSearch
	// INPUT: 5 ints
	// OUTPUT: 1 bool (true if a solution exists, false otherwise)
	// DESCRIPTION: This code follows the guidelines of the Lecture 5 
	// slides however, to keep track of the shortest path, it uses a
	// 2d array of ints which initially hold a large value but, as solutions
	// are found, the large ints are replaced by smaller ints and then
	// we find the solution with the smallest step length
	// Code adapted from Lecture 5 slides and Lee's algorithm for 
	// finding the shortest path in a maze
	int protestorQueueMazeSearch(int targetX, int targetY, int exitX, int exitY);

private:
	int m_numSquaresToMove;
	int m_ticksToWaitBetweenMoves;
	int m_ticks;
	int m_nonRestingTicks;
	int m_nonRestingPerpindicularTicks;
	// true = normal
	// false = leave-the-oil-field
	bool m_leaveTheOilFieldState;
};

// REGULARPROTESTOR

class RegularProtestor : public Protestor
{
public:
	RegularProtestor(int x, int y, StudentWorld* studentWorld);

	// NAME: doSomething
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: Regular Protesters, unlike the player, don’t necessarily get to take an action during every tick of the game (this is to make the game easier to play, since if Regular Protesters moved once every tick, they’d move 
	// much faster than the typical player can think and hit the keys on the keyboard). The Regular Protester must therefore compute a value indicating how often they’re allowed to take an action (e.g., once every N ticks). 
	// This number of ticks (also known as “resting ticks”) may be computed as follows: 
	// int ticksToWaitBetweenMoves = max(0, 3 –current_level_number / 4) 
	// So if the value of ticksToWaitBetweenMoves was 3, then the Regular Protester must “rest” for 3 ticks and may perform its normal behavior every 4th tick. If the value is zero, then the 
	// Regular Protester does something during every tick of the game. Here is what a Regular Protester must do during* every* tick:
	// 1. The Regular Protester must check to see if it is currently alive. If not, then its doSomething() method must return immediately –none of the following steps should be performed.
	// 2. If the Regular Protester is in a “rest state” during the current tick, it must do nothing other than update its resting tick count, and immediately return.
	// 3. Otherwise, if the Regular Protester is in a leave-the-oil-field state (because their hit points reached zero due to being repeatedly squirted by the TunnelMan’s squirt gun or bonked by a falling Boulder), then: 
		// a. If the Regular Protester is at its exit point (at location x = 60, y = 60) then it will immediately set its status to dead so that it will be removed from the game at the end of the current tick (by your StudentWorld class).
		// b. Otherwise, the Regular Protester must move one square closer to its exit point (at x = 60, y = 60). It must determine what direction to move based on a queue-based maze-searching algorithm like the one we learned in class.
		// The Regular Protester may not walk through Earth or Boulders to reach its exit point (meaning that none of the 16 squares that comprise the 4x4 Regular Protester icon may overlap with an Earthobject and that the Regular 
		// Protester must not come within (within means <= ) a distance of 3.0 units of any Boulders).
		// Hint: It’s inefficient for every single RegularProtester to try to figure out its own optimal path to the exit, so see if you can figure out a way to compute a single data structure that enables all Protesters to 
		// quickly determine their optimal path to their common exitpoint of 60, 60, no matter where they are located in the oil field.
		// c. After moving one step toward their exit point, the Regular Protester will return immediately.
		// It will not perform any of the following steps.
	// 4. Otherwise, the Regular Protester will check to see if they are within a distance of 4 units of the TunnelMan, AND they are currently facing in the TunnelMan’s direction. 
		// If both are true and the Regular Protester hasn’t shouted within its last non-resting 15 ticks, then the Regular Protester will: 
		// a. Play the shouting sound: SOUND_PROTESTER_YELL
		// b. Inform the TunnelMan that he’s been annoyed for a total of 2 annoyance points (deducting 2 points from the TunnelMan’s hit points, and possibly causing the TunnelMan to become sufficiently annoyed to quit the level (aka die)).
		// c. Update some state variable in a manner that prevents this RegularProtester from shouting again for at least 15 non-resting ticks.
		// d. Return immediately.
	// 5. Otherwise, if the Regular Protester:
		// a. Is in a straight horizontal or vertical line of sight to the TunnelMan(even if the Regular Protester isn’t currently facing the TunnelMan), and 
		// b. Is more than 4 units away from the TunnelMan –that is, the radius from the Regular Protester and theTunnelMan is greater than 4.0 units away, and
		// c. Could actually move the entire way to the TunnelMan with no Earth or Boulders blocking its path (assuming it kept walking straight over the next N turns) 
		// then the Regular Protester will:
			// a. Change its direction to face in the direction of the TunnelMan, AND then take one step toward him.
			// b. The Regular Protester will set its numSquaresToMoveInCurrentDirection value to zero, forcing it to pick a new direction/distance to move during 
			// its next non-resting tick (unless of course, the Regular Protester still sees the TunnelMan in its line of sight, in which case it will continue to move toward the TunnelMan).
			// c. Then the Regular Protester will immediately return.
	// 6. Otherwise, the Regular Protester can’t directly see the TunnelMan. As such, it will decrement its numSquaresToMoveInCurrentDirection variable by one. 
	// If the Regular Protester has finished walking numSquaresToMoveInCurrentDirection steps in its currently-selected direction (i.e.,numSquaresToMoveInCurrentDirection <= 0), then:
		// a. The Regular Protester will pick a random new direction to move (up, down, left or right).
		// b. If the random direction is blocked either by Earth or a Boulder such that it can’t take even a single step in that chosen direction, then it will select a different direction and check it for
		// blockage (it will continue checking directions until it has picked a direction that is not blocked).
		// c. The Regular Protester will then change its direction to this new chosen direction.
		// d. The Regular Protester will then pick a new value for numSquaresToMoveInCurrentDirection that will govern how far it should move in the selected direction over the next numSquaresToMoveInCurrentDirection non-resting ticks.
		// The value must be such that: 8 <= numSquaresToMoveInCurrentDirection<= 60.
		// e. The Regular Protester will then continue with step 8.
	// 7. Otherwise, if the Regular Protester:	
		// a. Is sitting at an intersection where it could turn and move at least one square in a perpendicular direction from its currently facing direction (e.g., it is currently facing left, and is at a junction where it could turn
		// and move one step either upward/downward without being blocked by Earth or a Boulder), and
		// b. The Regular Protester hasn’t made a perpendicular turn in the last 200 non-resting ticks. Then the Regular Protester will:
			// a. Determine which of the two perpendicular directions are viable (a viable direction is one that allows movement of at least one square without the Regular Protester being blocked by Earth ora Boulder).
			// b. Pick a viable perpendicular direction. If both perpendicular directions are viable, then pick one of the two choices randomly.
			// c. Set its direction to the selected perpendicular direction.
			// d. Pick a new value for numSquaresToMoveInCurrentDirectionthat will govern how far it should move in the selected perpendicular direction over the next numSquaresToMoveInCurrentDirection non-resting ticks.
			// The value must be such that: 8 <= numSquaresToMoveInCurrentDirection <= 60.
		// e. Remember that it made a perpendicular turn so that it doesn’t make another perpendicular turn for at least 200 more non-resting ticks.
		// f. Continue on with the next step.
	// 8. Finally, the Regular Protester will then attempt to take one step in its currently facing direction (which, by the way, might have just been changed by one of the last few steps above).
	// 9. If the Regular Protester is for some reason blocked from taking a step in its currently facing direction, it will set the numSquaresToMoveInCurrentDirection to zero, resulting in a new direction being chosen during 
	// the Regular Protester’s next non-resting tick (but not the current tick –the Regular Protester must do nothing during the current tick).
	// Note: Only Earth and Boulders block a Regular Protester from moving. RegularProtesters can occupy the same square as all other game objects including theTunnelMan and other Protesters.
	// pg. 39/40/41/42/43
	virtual void doSomething();

	// NAME: bribed
	// INPUT: NONE
	// OUTPUT: NONE 
	// DESCRIPTION: puts the protestor into a leave-the-oil-field-state and
	// plays the correct sound
	virtual void bribed();

private:
	
};

// HARDCOREPROTESTOR 

class HardcoreProtestor : public Protestor
{
public:
	HardcoreProtestor(int x, int y, StudentWorld* studentWorld);

	// NAME: doSomething
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: Hardcore Protesters, unlike the player, don’t necessarily get to take an action during every tick of the game (this is to make the game easier to play, since if Hardcore Protesters moved once every tick, they’d move 
	// much faster than the typical player can think and hit the keys on the keyboard). The Hardcore Protester must therefore compute a value indicating how often they’re allowed to take an action (e.g., once every N ticks). 
	// This number of ticks (also known as “resting ticks”) may be computed as follows: 
	// int ticksToWaitBetweenMoves = max(0, 3 –current_level_number / 4) 
	// So if the value of ticksToWaitBetweenMoveswas 3, then the Hardcore Protester must “rest” for 3 ticks and may perform its normal behavior every 4th tick. If the value is zero, then the 
	// Hardcore Protester does something during every tick of the game. Here is what a Hardcore Protester must do during* every* tick:
	// 1. The Hardcore Protester must check to see if it is currently alive. If not, then its doSomething() method must return immediately –none of the following stepsshould be performed.
	// 2. If the Hardcore Protester is in a “rest state” during the current tick, it must do nothing other than update its resting tick count, and immediately return.
	// 3. Otherwise, if the Hardcore Protester is in a leave-the-oil-field state (because their hit points reached zero due to being repeatedly squirted by the TunnelMan’s squirt gun or bonked by a falling Boulder), then: 
		// a. If the Hardcore Protester is at its exit point (at location x = 60, y = 60) then it will immediately set its status to dead so that it will be removed from the game at the end of the current tick (by your StudentWorld class).
		// b. Otherwise, the Hardcore Protester must move one square closer to its exit point (at x = 60, y = 60). It must determine what direction to move based on a queue-based maze-searching algorithm like the one we learned in class.
		// The Hardcore Protester may not walk through Earth or Boulders to reach its exit point (meaning that none of the 16 squares that comprise the 4x4 Hardcore Protester icon may overlap with an Earth object and that the Hardcore 
		// Protester must not come within (within means <= ) a distance of 3.0 units of any Boulders).
		// Hint: It’s inefficient for every single Hardcore Protestor to try to figure out its own optimal path to the exit, so see if you can figure out a way to compute a single data structure that enables all Protesters to 
		// quickly determine their optimal path to their common exitpoint of 60, 60, no matter where they are located in the oil field.
		// c. After moving one step toward their exit point, the Hardcore Protester will return immediately.
		// It will not perform any of the following steps.
	// 4. Otherwise, the Hardcore Protester will check to see if they are within a distance of 4 units of the TunnelMan, AND they are currently facing in the TunnelMan’s direction. 
	// If both are true and the Hardcore Protester hasn’t shouted within its last non-resting 15 ticks, then the Hardcore Protester will: 
		// a. Play the shouting sound: SOUND_PROTESTER_YELL
		// b. Inform the TunnelMan that he’s been annoyed for a total of 2 annoyance points (deducting 2 points from the TunnelMan’s hit points, and possibly causing the TunnelMan to become sufficiently annoyed to quit the level (aka die)).
		// c. Update some state variable in a manner that prevents this Hardcore Protester from shouting again for at least 15 non-resting ticks.
		// d. Return immediately.
	// 5. Otherwise, if the Hardcore Protester is more than 4.0 units away from the TunnelMan, the Hardcore Protester wil:
		// a. Compute a value M, such that M = 16 + current_level_number * 2
		// b. If the Hardcore Protester is less than or equal to a total of M legal horizontal or vertical moves away from the current location of the TunnelMan (as can be determined using the same 
		// basic queue-based maze-searching algorithm described in Hardcore Protester item 3.b above), then the Hardcore Protester will sense the TunnelMan’s cell phone signal and know where he is. 
		// This is true even if the Hardcore Protester has no direct line of sight to the TunnelMan, so long as he can be reached in M moves without digging through Earth or going through a Boulder. 
		// In such a situation, the Hardcore Protester will:
			// i. Determine which horizontal/vertical direction to move in (as dictated by its maze-searching algorithm) such that if it were to make such a move it would be one square closer to the TunnelMan.
			// ii. Change its current direction to face this new direction.
			// iii. Move one square in this direction such that after the move it is one square closer to the TunnelMan’s current location.
			// iv. Return immediately
	// 6. Otherwise, if the Hardcore Protester:
		// a. Is in a straight horizontal or vertical line of sight to the TunnelMan (even if the Hardcore Protester isn’t currently facing the TunnelMan), and 
		// b. Is more than 4 units away from the TunnelMan –that is, the radius from the Hardcore Protester and the TunnelMan is greater than 4.0 units away, and
		// c. Could actually move the entire way to the TunnelMan with no Earth or Boulders blocking its path (assuming it kept walking straight over the next N turns) 
		// then the Hardcore Protester will:
			// a. Change its direction to face in the direction of the TunnelMan, AND then take one step toward him.
			// b. The Hardcore Protester will set its numSquaresToMoveInCurrentDirection value to zero, forcing it to pick a new direction/distance to move during 
			// its next non-resting tick (unless of course, the Hardcore Protester still sees the TunnelManin its line of sight, in which case it will continue to move toward the TunnelMan).
			// c. Then the Hardcore Protester will immediately return.
	// 7. Otherwise, the Hardcore Protester can’t directly see the TunnelMan. As such, it will decrement its numSquaresToMoveInCurrentDirection variable by one. If the Hardcore Protester has finished walking
	// numSquaresToMoveInCurrentDirection steps in its currently-selected direction (i.e.,numSquaresToMoveInCurrentDirection <= 0), then:
		// a. The Hardcore  Protester will  pick  a  random  new  direction  to  move  (up, down, left or right).
		// b. If the random direction is blocked either by Earth or a Boulder such that it can’t take even a single step in that chosen direction, then it will select a different direction and check it for
		// blockage (it will continue checking directions until it has picked a direction that is not blocked).
		// c. The Hardcore Protester will then change its direction to this new chosen direction.
		// d. The Hardcore Protester will then pick a new value for numSquaresToMoveInCurrentDirection that will govern how far it should move in the selected direction over the next numSquaresToMoveInCurrentDirection non-resting ticks.
		// The value must be such that: 8 <= numSquaresToMoveInCurrentDirection<= 60.
		// e. The Hardcore Protester will then continue with step 8.
	// 8. Otherwise, if the Hardcore Protester:	
		// a. Is sitting at an intersection where it could turn and move at least one square in a perpendicular direction from its currently facing direction (e.g., it is currently facing left, and is at a junction where it could turn
		// and move one step either upward/downward without being blocked by Earth or a Boulder), and
		// b. The Hardcore Protester hasn’t made a perpendicular turn in the last 200 non-resting ticks. 
		// Then the Hardcore Protester will:
			// a. Determine which of the two perpendicular directions are viable (a viable direction is one that allows movement of at least one square without the Hardcore Protester being blocked by Earth ora Boulder).
			// b. Pick a viable perpendicular direction. If both perpendicular directions are viable, then pick one of the two choices randomly.
			// c. Set its direction to the selected perpendicular direction.
			// d. Pick a new value for numSquaresToMoveInCurrentDirectionthat will govern how far it should move in the selected perpendicular direction over the next numSquaresToMoveInCurrentDirection non-resting ticks.
			// The value must be such that: 8 <= numSquaresToMoveInCurrentDirection <= 60.
			// e. Remember that it made a perpendicular turn so that it doesn’t make another perpendicular turn for at least 200 more non-resting ticks.
			// f. Continue on with the next step.
	// 9. Finally, the Hardcore Protester will then attempt to take one step in its currently facing direction (which, by the way, might have just been changed by one of the last few steps above).
	// 10. If the Hardcore Protester is for some reason blocked from taking a step in its currently facing direction, it will set the numSquaresToMoveInCurrentDirection to zero, resulting in a new direction being chosen during 
	// the Hardcore Protester’s next non-resting tick (but not the current tick –the Hardcore Protester must do nothing during the current tick).
	// Note: Only Earth and Boulders block a Hardcore Protester from moving. Hardcore Protesters can occupy the same square as all other game objects including the TunnelMan and other Protesters.
	// pg. 44/45/46/47
	virtual void doSomething();

	// NAME: bribed
	// INPUT: NONE
	// OUTPUT: NONE 
	// DESCRIPTION: puts the protestor into a leave-the-oil-field-state and
	// plays the correct sound
	virtual void bribed();

private:
};

#endif // ACTOR_H_
