#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include "Actor.h"
class Actor;
class Earth;
class TunnelMan;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

// STUDENTWORLD 

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);

	// see cleanUp()
	virtual ~StudentWorld();

	// NAME: init
	// INPUT: NONE
	// OUTPUT: Returns game status (continue)
	// DESCRIPTION: Your StudentWorld’s init() method must:
	// 1. Initialize the data structures used to keep track of your game’s virtual world
	// 2. Construct a new oil field that meets the requirements stated in the section below (filled with Earth, Barrels of oil, Boulders, Gold Nuggets, etc.)
	// 3. Allocate and insert a valid TunnelMan object into the game world at the proper location
	// 4. Your init() method must construct a representation of your virtual world and store this in your StudentWorld object. 
	// It is required that you keep track of all of the game objects (e.g., actors like Regular Protesters, Gold Nuggets, Barrels of oil, Sonar Kits, Boulders, etc.) 
	// with the exception of Earth objects and the TunnelMan object in a single STL collection like a list or vector. 
	// To do so, we recommend using a vector of pointers to your game objects, or a list of pointers to your game objects.
	// pg. 15/16
	virtual int init();

	// NAME: move
	// INPUT: NONE
	// OUTPUT: Returns game status (continue, player_died, etc)
	// DESCRIPTION: The move() method must perform the following activities:
	// 1. It must update the status text on the top of the screen with the latest information (e.g., the user’s current score, the remaining bonus score for the level, etc.).
	// 2. It must ask all of the actors that are currently active in the game world to do something (e.g., ask a Regular Protesterto move itself, ask a Boulder to see if it needs to fall down because Earth beneath it was 
	// dug away, give the TunnelMana chance to move up, down, left or right, etc.).
		// a. If an actor does something that causes the TunnelMan to give up, then the move() method should immediately return GWSTATUS_PLAYER_DIED.
		// b. If the TunnelMan collects all of the Barrels of oil on the level (completing the current level), then the move() method should immediately play a sound of SOUND_FINISHED_LEVEL and 
		// then return a value of GWSTATUS_FINISHED_LEVEL.
	// 3. It must then delete any actors that need to be removed from the game during this tick and remove them from your STL container that tracks them. This includes, for example:
		// a. A Protester that has run to the upper-right-hand corner of the oil field after being sufficiently annoyed (by being squirted by a Squirt or hit by a Boulder) and is ready to “leave” the oil field
		// b. A Boulder that has fallen down a shaft and disintegrated upon hitting the bottom (or another Boulder)
		// c. A Gold Nuggetthat has been picked up by the TunnelMan or a Protester and is therefore no longer in the oil field
		// d. A Water Pool that has dried up after a period of time.
		// e. A Squirt from the TunnelMan’s squirt gun once it’s reached the maximum distance it can travel.
	// pg. 18
	virtual int move();

	// NAME: cleanUp
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: Deletes all of the actors from the 
	// game by iterating through the actors vector, deletes all
	// of the earth pointers , sets the virtual world 2d array to 
	// empty, and deletes the tunnelman (destructor uses this function)
	virtual void cleanUp();

	// NAME: randomCoordinates
	// INPUT: 2 ints passed by reference
	// OUTPUT: NONE (changes the 2 ints)
	// DESCRIPTION: Create random coordinates to place game objects
	// No distributed game object may be within a radius (Euclidian distance) of
	// 6 squares of any other distributed game object 
	// pg. 17
	void randomCoordinates(int& x, int& y);

	// NAME: getVirtualWorld
	// INPUT: 2 ints 
	// OUTPUT: 1 int 
	// DESCRIPTION: Returns the value held in the 2d virtual world array
	// at the coordinates (x,y) 
	int getVirtualWorld(int x, int y);

	// NAME: digEarth
	// INPUT: 2 ints 
	// OUTPUT: NONE
	// DESCRIPTION: Checks for and clears Earth objects
	// in a 4x4 radius around the cooordinates (x,y) by setting
	// the virtual world 2d array to 0 and deleting the earth 
	// pointers in the earth pointers 2d array
	void digEarth(int x, int y);

	// NAME: annoyProtestors
	// INPUT: 3 ints 
	// OUTPUT: 1 bool (true if successful, false otherwise)
	// DESCRIPTION: Attempts to annoy protestors for a total of whatever
	// amount is passed in as an input by checking if a protestor is within a
	// radius of 3 of the coordinates (x,y)
	bool annoyProtestors(int x, int y, int amount);

	// NAME: distanceToTunnelMan
	// INPUT: 2 ints 
	// OUTPUT: 1 double
	// DESCRIPTION: Returns the distance from the object at
	// coordinates (x,y) to the tunnelman
	double distanceToTunnelMan(int x, int y);

	// NAME: isPathClear
	// INPUT: 3 ints
	// OUTPUT: 1 bool 
	// DESCRIPTION: Checks a straight line path (based on which direction is passed in)
	// to see if we can spot the tunnelman from the current (x,y) 
	// Used by the protestors to determine movement
	bool isPathClear(int x, int y, int direction);

	// NAME: isEmpty
	// INPUT: 2 ints
	// OUTPUT: 1 bool
	// DESCRIPTION: Returns true if the 4x4 radius around the coordinates 
	// has a boulder or earth, false otherwise
	bool isEmpty(int x, int y);

	// NAME: isBoulder
	// INPUT: 2 ints
	// OUTPUT: 1 bool
	// DESCRIPTION: Returns true if the 4x4 radius around the coordinates 
	// has a boulder, false otherwise
	bool isBoulder(int x, int y);

	// NAME: isEarth
	// INPUT: 2 ints
	// OUTPUT: 1 bool
	// DESCRIPTION: Returns true if the 4x4 radius around the coordinates 
	// has an earth object, false otherwise
	bool isEarth(int x, int y);

	// NAME: setEmpty
	// INPUT: 2 ints
	// OUTPUT: NONE
	// DESCRIPTION: Removes all earth and boulder objects in a 4x4 radius 
	// around the coordinates 
	void setEmpty(int x, int y);

	// NAME: updateDisplayText
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: Update the score/lives/level text at screen top (pg. 19)
	void updateDisplayText();

	// NAME: placeGoldNugget
	// INPUT: 3 ints
	// OUTPUT: NONE
	// DESCRIPTION: Places a new gold nugget at the coordinates (x,y) 
	// and sets the state
	void placeGoldNugget(int x, int y, int state);

	// NAME: removeDeadGameObjects
	// INPUT: NONE
	// OUTPUT: NONE
	// DESCRIPTION: Remove newly-dead actors after each tick 
	// delete dead game objects
	void removeDeadGameObjects();

	// NAME: squirt
	// INPUT: 2 ints and 1 direction
	// OUTPUT: NONE
	// DESCRIPTION: Squirt at the coordinates (x,y) and in the direction
	// provided by the input after checking certain conditions
	void squirt(int x, int y, GraphObject::Direction direction);

	// NAME: bribeNearbyProtestor
	// INPUT: 2 ints
	// OUTPUT: 1 bool (true if a protestor was bribed, false otherwise)
	// DESCRIPTION: Checks for protestors in a 3x3 radius of
	// the coordinates (x,y) and, if it finds one, bribes them
	// by calling the actor function "bribed" and increases the player's 
	// score based on the type of protestor
	bool bribeNearbyProtestor(int x, int y);

	// NAME: useSonarCharge
	// INPUT: 2 ints
	// OUTPUT: NONE
	// DESCRIPTION: Checks for actos in a 12x12 radius around
	// the coordinates (x,y) and makes them all visible
	void useSonarCharge(int x, int y);

	// accessors

	// NAME: getBarrelsOfOilInField
	// INPUT: NONE
	// OUTPUT: 1 int  
	// DESCRIPTION: Returns m_barrelsOfOilInField 
	int getBarrelsOfOilInField();

	// NAME: getTunnelMan
	// INPUT: NONE
	// OUTPUT: 1 pointer to a tunnelman 
	// DESCRIPTION: Returns m_tunnelManPointer
	TunnelMan* getTunnelMan() const;

private:
	std::vector<Actor*> actorsVector;
	TunnelMan* m_tunnelManPointer;
	Earth* earthPointers[64][64];
	// 0 = Empty
	// 1 = Earth
	// 2 = Boulder
	int virtualWorld[64][64];
	int m_barrelsOfOilInField;
	int m_protestors;
	int m_ticks;
	int m_protestorTicks;
};

#endif // STUDENTWORLD_H_
