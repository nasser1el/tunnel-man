#include "StudentWorld.h"
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}


// STUDENTWORLD IMPLEMENTATIONS //

// Create a limited version of the StudentWorldclass. (PART 1)
// 1. Add any private member variables to this class required to keep track of all Earth in the oil field as well as the TunnelMan object.
// You may ignore all other items in the oil field such as Boulders, Barrels of oil, Protesters, Nuggets, etc. for part #1.
// 2. Implement a  constructor for this class that initializes all member variables required for proper gameplay.
// 3. Implement a destructor for this class that frees any remaining dynamically allocated data that has not yet been freed at the time the class is destroyed (e.g., the TunnelMan and all remaining Earth).
// 4. Implement the init() method in this class. It must: 
// a. Create the TunnelMan object and insert it into the oil field at the right starting location (see the StudentWorld init()section of this document for details on the starting location).
// b. Create all of the oil field’s Earth objects and inserts them into a data structure that tracks active Earth 
// (see the StudentWorld init() section for details on where to place Earth, and what data structure to use track all of the remaining Earth in the game)
// 5. Implement the move() method in your StudentWorld class. During each tick, it must ask your TunnelMan object to do something. Your move() method need not check to see if the TunnelMan 
// has died or not; you may assume at this point that the TunnelMan cannot die. Nor need your move() method deal with any Protesters or other actors(e.g., Nuggets or Boulders) at this point –just the TunnelMan.
// 6. Implement a cleanup() method that frees any dynamically allocated data that was allocated during calls to the init()method or the move() method(e.g., it should delete all your allocated Earth and the TunnelMan).
// Note: Your StudentWorld class must have both a destructor and the cleanUp() method even though they likely do the same thing.
// pg. 51/52

// DONE: 1, 2, 3, 4, 5, 6
// TODO: 
StudentWorld::StudentWorld(string assetDir)
	: GameWorld(assetDir)
{
	m_protestors = 0;
	m_ticks = 0;
	m_tunnelManPointer = nullptr;
	m_protestorTicks = 0;
	m_barrelsOfOilInField = 0;
	// Initialize a vector of actors
	actorsVector.clear();

	// Initialize a 2d array of earth pointers to null and initialize
	// a 2d array of the virtual world to 0 (empty)
	for (int x = 0; x != 64; x++)
	{
		for (int y = 0; y != 64; y++)
		{
			earthPointers[x][y] = nullptr;
			virtualWorld[x][y] = 0;
		}
	}
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

// NAME: getTunnelMan
// INPUT: NONE
// OUTPUT: 1 pointer to a tunnelman 
// DESCRIPTION: Returns m_tunnelManPointer
TunnelMan* StudentWorld::getTunnelMan() const
{
	return m_tunnelManPointer;
}


// NAME: getBarrelsOfOilInField
// INPUT: NONE
// OUTPUT: 1 int  
// DESCRIPTION: Returns m_barrelsOfOilInField 
int StudentWorld::getBarrelsOfOilInField()
{
	return m_barrelsOfOilInField;
}

// NAME: isPathClear
// INPUT: 3 ints
// OUTPUT: 1 bool 
// DESCRIPTION: Checks a straight line path (based on which direction is passed in)
// to see if we can spot the tunnelman from the current (x,y) 
// Used by the protestors to determine movement
// 1 = up
// 2 = down
// 3 = left
// 4 = right
bool StudentWorld::isPathClear(int x, int y, int direction)
{
	if (direction == 1)
	{
		for (int k = 0; y + k <= 60; k++)
		{
			// Check if there is a boulder or earth in the way
			if (getVirtualWorld(x, y + k) != 0)
				return false;

			// Check if we can see the tunnelman from here
			if (m_tunnelManPointer->getX() == x && m_tunnelManPointer->getY() == y + k)
				return true;
		}
	}
	if (direction == 2)
	{
		for (int k = 0; y - k >= 60; k++)
		{
			if (getVirtualWorld(x, y-k) != 0)
				return false;
			if (m_tunnelManPointer->getX() == x && m_tunnelManPointer->getY() == y - k)
				return true;
		}
	}
	if (direction == 3)
	{
		for (int k = 0; x - k >= 0; k++)
		{
			if (getVirtualWorld(x - k, y) != 0)
				return false;
			if (m_tunnelManPointer->getX() == x - k && m_tunnelManPointer->getY() == y)
				return true;
		}
	}
	if (direction == 4)
	{
		for (int k = 0; x + k <= 63; k++)
		{
			if (getVirtualWorld(x + k, y) != 0)
				return false;
			if (m_tunnelManPointer->getX() == x + k && m_tunnelManPointer->getY() == y)
				return true;
		}
	}

	// Could not see the tunnelman so return false
	return false;
}

// NAME: isEmpty
// INPUT: 2 ints
// OUTPUT: 1 bool
// DESCRIPTION: Returns true if the 4x4 radius around the coordinates 
// has a boulder or earth, false otherwise
bool StudentWorld::isEmpty(int x, int y)
{
	for (int i = x; i != x + 4; i++)
	{
		int k = y;
		while (k != y + 4)
		{
			if (virtualWorld[i][k] != 0)
				return false;
			k++;
		}
	}

	return true;
}

// NAME: isBoulder
// INPUT: 2 ints
// OUTPUT: 1 bool
// DESCRIPTION: Returns true if the 4x4 radius around the coordinates 
// has a boulder, false otherwise
bool StudentWorld::isBoulder(int x, int y)
{
	for (int i = x; i != x + 4; i++)
	{
		int k = y;
		while (k != y + 4)
		{
			if (virtualWorld[i][k] == 2)
				return true;
			k++;
		}
	}

	return false;
}

// NAME: isEarth
// INPUT: 2 ints
// OUTPUT: 1 bool
// DESCRIPTION: Returns true if the 4x4 radius around the coordinates 
// has an earth object, false otherwise
bool StudentWorld::isEarth(int x, int y)
{
	for (int i = x; i != x + 4; i++)
	{
		int k = y;
		while (k != y + 4)
		{
			if (virtualWorld[i][k] == 1)
				return true;
			k++;
		}
	}

	return false;
}

// NAME: setEmpty
// INPUT: 2 ints
// OUTPUT: NONE
// DESCRIPTION: Removes all earth and boulder objects in a 4x4 radius 
// around the coordinates 
void StudentWorld::setEmpty(int x, int y)
{
	for (int i = x; i != x + 4; i++)
	{
		int k = y;
		while (k != y + 4)
		{
			virtualWorld[i][k] = 0;
			k++;
		}
	}
}

// NAME: updateDisplayText
// INPUT: NONE
// OUTPUT: NONE
// DESCRIPTION: Update the score/lives/level text at screen top (pg. 19)
void StudentWorld::updateDisplayText()
{
	int level = getLevel(); 
	int lives = getLives();
	int health = m_tunnelManPointer->getHitPoints() * 10; // turn hitpoints into a percent
	int squirts = m_tunnelManPointer->getSquirtsLeftInSquirtGun(); 
	int gold = m_tunnelManPointer->getPlayerGoldCount(); 
	int barrelsLeft = m_tunnelManPointer->getNumberOfBarrelsRemainingToBePickedUp();
	int sonar = m_tunnelManPointer->getPlayerSonarChargeCount();
	int score = getScore();
	
	// Next, create a string from your statistics, of the form:
	// Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000 
	
	// From stringstream file uploaded to CCLE
	ostringstream oss;
	oss.setf(ios::fixed);
	oss.precision(0);

	// Each field must be exactly as wide as shown in the example above:
		// a. The Lvlfield must be 2 digits long, with leading spaces (e.g., “_1”, where _ represents a space).
		// b. The Lives field should be 1 digit long (e.g., “2”).
		// c. The Hlth field should be 3 digits long and display the player’s health percentage (not its hit-points!), with leading spaces, and be followed by a percent sign (e.g., “_70%”).
		// d. The Wtr field should be 2 digits long, with a leading space as required (e.g., “_ 7”).
		// e. The Gld field should be 2 digits long, with a leading space as required (e.g., “_ 3”).
		// f. The Oil Left fieldshould be 2 digits long, with a leading space as required (e.g., “_ 4”).
		// g. The Sonar field should be 2 digits long, with a leading space as required (e.g., “_ 2”).
		// h. The Scr must be exactly 6 digits long, with leading zeros (e.g., 003124).
	// pg. 22
	oss << "Lvl: " << setw(2) << level << " Lives: " << setw(1) << lives << " Hlth: " << setw(3) <<
		health << "%" << " Wtr: " << setw(2) << squirts << " Gld: " << setw(2) << gold <<
		" Oil Left: " << setw(2) << barrelsLeft  << " Sonar: " << setw(2) << sonar <<
		" Scr: " << oss.fill('0') << setw(6) << score;

	// string s = someFunctionYouUseToFormatThingsNicely(level, lives, health, squirts, gold, barrelsLeft, sonar, score);
	string s = oss.str();

	// Finally, update the display text at the top of the screen with your
	// newly created stats
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

// NAME: placeGoldNugget
// INPUT: 3 ints
// OUTPUT: NONE
// DESCRIPTION: Places a new gold nugget at the coordinates (x,y) 
// and sets the state
void StudentWorld::placeGoldNugget(int x, int y, int state)
{
	GoldNugget* newGoldNugget = new GoldNugget(x, y, this, state);
	actorsVector.push_back(newGoldNugget);
}


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

// DONE: 1, 2, 3, 4
// TODO: 
int StudentWorld::init()
{
	m_protestors = 0;
	m_ticks = 0;
	m_barrelsOfOilInField = min(int(2 + getLevel()), 21);

	// Initialize an vector of actors
	actorsVector.clear();

	// Create a pointer to the tunnelman
	m_tunnelManPointer = new TunnelMan(this);

	// Fill the oil field with Earth objects 
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 60; y++)
		{
			if ((x < 30 || x > 33) || (y < 4 || y > 59))
			{
				earthPointers[x][y] = new Earth(x, y, this);
				virtualWorld[x][y] = 1;
			}
			else 
				virtualWorld[x][y] = 0;
		}
	}

	// Distribute B Boulders, G Gold Nuggets, and L Barrels
	// of oil (pg. 17)
	int B = min(int(getLevel() / 2 + 2), 9);
	int G = max(int(5 - getLevel() / 2), 2);
	int L = min(int(2 + getLevel()), 21);
	
	for (int k = 0; k != B; k++)
	{
		int x, y;
		randomCoordinates(x, y);

		// Make sure the boulders are only where there is Earth
		while (isEarth(x,y) == false)
			randomCoordinates(x, y);
		for (int a = x; a < x + 4; a++)
		{
			int b = y;
			while (b < y + 4)
			{
				if (earthPointers[a][b] != nullptr)
				{
					delete earthPointers[a][b];
					earthPointers[a][b] = nullptr;
					virtualWorld[a][b] = 2;

					actorsVector.push_back(new Boulder(x, y, this));
				}

				b++;
			}
		}		
	}

	int c = 0;
	while (c != G)
	{
		int x, y;
		randomCoordinates(x, y);

		// make sure the gold nuggets are only where there is Earth
		while (isEarth(x, y) == false)
		randomCoordinates(x, y);
		GoldNugget* newGoldNugget = new GoldNugget(x, y, this, 0);
		actorsVector.push_back(newGoldNugget);
		c++;
	}

	int i = 0;
	while (i != L)
	{
		int x, y;
		randomCoordinates(x, y);

		// make sure the oil barrels are only where there is Earth
		while (isBoulder(x, y) == true && isEmpty(x, y) == true)
			randomCoordinates(x, y);
		BarrelOfOil* newBarrelOfOil = new BarrelOfOil(x, y, this);
		actorsVector.push_back(newBarrelOfOil);

		i++;
	}

	// Game starts out with 1 regular protestor
	RegularProtestor* newRegularProtestor = new RegularProtestor(60, 60, this);
	actorsVector.push_back(newRegularProtestor);
	m_protestors++;

	return GWSTATUS_CONTINUE_GAME;
}

// NAME: removeDeadGameObjects
// INPUT: NONE
// OUTPUT: NONE
// DESCRIPTION: Remove newly-dead actors after each tick 
// delete dead game objects
void StudentWorld::removeDeadGameObjects()
{
	vector<Actor*>::iterator it = actorsVector.begin();
	while (it != actorsVector.end())
	{
		if ((*it)->getLifeStatus() == false)
		{
			delete (*it);
			it = actorsVector.erase(it);
		}
		else
			it++;
	}
}

// NAME: squirt
// INPUT: 2 ints and 1 direction
// OUTPUT: NONE
// DESCRIPTION: Squirt at the coordinates (x,y) and in the direction
// provided by the input after checking certain conditions
void StudentWorld::squirt(int x, int y, GraphObject::Direction direction)
{
	// Only run if the tunnelman has ammo
	if (getTunnelMan()->getSquirtsLeftInSquirtGun() > 0)
	{

		// Put a squirt into the actors vector
		Squirt* newSquirt = new Squirt(x, y, direction, this);
		actorsVector.push_back(newSquirt);
		playSound(SOUND_PLAYER_SQUIRT);

		// Decrement the squirts left
		getTunnelMan()->setSquirts(getTunnelMan()->getSquirtsLeftInSquirtGun() - 1);
	}
}

// NAME: bribeNearbyProtestor
// INPUT: 2 ints
// OUTPUT: 1 bool (true if a protestor was bribed, false otherwise)
// DESCRIPTION: Checks for protestors in a 3x3 radius of
// the coordinates (x,y) and, if it finds one, bribes them
// by calling the actor function "bribed" and increases the player's 
// score based on the type of protestor
bool StudentWorld::bribeNearbyProtestor(int x, int y)
{
	// Checks for and annoys Protestors in a 3x3 radius
	vector<Actor*>::iterator it;
	for (it = actorsVector.begin(); it != actorsVector.end(); it++)
	{
		if ((*it)->getX() >= (x - 3) && (*it)->getX() <= (x + 3))
		{
			if ((*it)->getY() >= (y - 3) && (*it)->getY() <= (y + 3))
			{
				// Check if the actor is a protestor and if it's alive
				if ((*it)->getProtestor() != 0 && (*it)->getLifeStatus() == true)
				{
					(*it)->bribed();
					if ((*it)->getProtestor() == 1)
						increaseScore(25);
					else
						increaseScore(50);
					return true;
				}
			}
		}
	}

	return false;
}

// NAME: useSonarCharge
// INPUT: 2 ints
// OUTPUT: NONE
// DESCRIPTION: Checks for actos in a 12x12 radius around
// the coordinates (x,y) and makes them all visible
void StudentWorld::useSonarCharge(int x, int y)
{
	vector<Actor*>::iterator it;
	playSound(SOUND_SONAR);

	for (it = actorsVector.begin(); it != actorsVector.end(); it++)
	{
		// If the actor is within a radius of 12 of the coordinates
		if (sqrt(pow(x - ((*it)->getX()), 2) + pow(y - ((*it)->getY()), 2)) <= 12.0)
		{
			(*it)->setVisible(true);
		}
	}
}

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

// DONE: 1, 2a, 2b, 3
// TODO: 
int StudentWorld::move()
{
	// During each tick of the game in your move() method, you may need to add new Protesters (Regular or Hardcore)
	// and/or Goodies (Water Pools or Sonar Kits) to the oil field. 
	m_protestorTicks++;

	// Add protestor only if a certain number of ticks (T) have passed and there are less
	// than P protestors in the field
	int T = max(25, int(200 - getLevel()));
	int P = min(15, int(2 + getLevel() * 1.5));
	if (m_protestorTicks >= T && m_protestors < P)
	{
		// Probability of a hardcore protestor spawning ranges from 30-90%
		int probabilityOfHardcore = min(90, int(getLevel() * 10 + 30));
		int prob = rand() % 100 + 1;

		// Check the probabilityOfHardcore against a random number from 1-100
		if (prob > probabilityOfHardcore)
		{
			RegularProtestor* newRegularProtestor = new RegularProtestor(60, 60, this);
			actorsVector.push_back(newRegularProtestor);
		}
		else
		{
			HardcoreProtestor* newHardcoreProtestor = new HardcoreProtestor(60, 60, this);
			actorsVector.push_back(newHardcoreProtestor);
		}

		// Increase number of protestors counter and reset protestor tick counter
		m_protestors++;
		m_protestorTicks = 0;
	}

	// 1 in G chance of a SonarCharge or WaterPool spawning
	int G = getLevel() * 25 + 300;
	int prob = rand() % G + 1;
	if (prob <= 1)
	{
		// 1 in 5 chance of a SonarCharge spawning
		int sonarOrWater = rand() % 100 + 1;
		if (sonarOrWater > 20)
		{
			int x, y;
			randomCoordinates(x, y);
			// Water can only be added to empty areas (no boulders or earth)
			while (isEmpty(x, y) == false)
				randomCoordinates(x, y);
			WaterPool* newWaterPool = new WaterPool(x, y, this);
			actorsVector.push_back(newWaterPool);
		}
		else
		{
			// Each new Sonar Kit must be added at x=0, y=60 on the screen
			SonarKit* newSonarKit = new SonarKit(0, 60, this);
			actorsVector.push_back(newSonarKit);
		}
	}

	m_tunnelManPointer->doSomething();

	/*This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.*/

	// Update the Game Status Line
	updateDisplayText(); 

	// The term “Actors” refers to all Protesters, the player, Goodies,
	// Boulders, Barrels of oil, Holes, Squirts, the Exit, etc.
	// Give each Actor a chance to do something

	vector<Actor*>::iterator it;

	for (it = actorsVector.begin(); it != actorsVector.end(); it++)
	{
		// If the actor is alive, allow it to perform an action
		if( (*it)->getLifeStatus() == true)
			(*it)->doSomething();

		// if (theplayerDiedDuringThisTick() == true) 
		if (m_tunnelManPointer->getLifeStatus() == false)
		{
			// If the player is dead, remove a life and return
			decLives();
			playSound(SOUND_PLAYER_GIVE_UP);
			return GWSTATUS_PLAYER_DIED;
		}
	}

	removeDeadGameObjects();

	// If the player has collected all of the Barrels on the level, then
	// return the result that the player finished the level
	// if (theplayerCompletedTheCurrentLevel() == true)
	// {
	//	 playFinishedLevelSound(); 
	//	 return GWSTATUS_FINISHED_LEVEL;
	// }
	// (From spec)

	if (getTunnelMan()->getNumberOfBarrelsRemainingToBePickedUp() <= 0)
	{
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	// the player hasn’t completed the current level and hasn’t died
	// let them continue playing the current level
	return GWSTATUS_CONTINUE_GAME;
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


// NAME: randomCoordinates
// INPUT: 2 ints passed by reference
// OUTPUT: NONE (changes the 2 ints)
// DESCRIPTION: Create random coordinates to place game objects
// No distributed game object may be within a radius (Euclidian distance) of
// 6 squares of any other distributed game object 
// pg. 17
void StudentWorld::randomCoordinates(int& x, int& y)
{
	int i = 0;

	// Break out of the while loop when i == 2
	while (i != 2)
	{
		i = 0;
		x = rand() % 59 + 1;
		y = rand() % 59 + 1;
		
		if (actorsVector.size() == 0)
			i = 2;

		// Loop until we reach the end of the actors vector
		int k = 0;
		while (k != actorsVector.size())
		{
			// If the distance between the random coordinates and another actor's coordinates is less than
			// or equal to 6, set i = 1;
			if ((sqrt(pow(x - (actorsVector[k]->getX()), 2) + pow(y - (actorsVector[k]->getY()), 2))) <= 6.0)
				i = 1;
			k++;
		}

		// If we got through the whole actors vector without setting i = 1, we know that the new coordinates
		// are not within a radius of 6 of other game objects
		if (i != 1)
			i = 2;
			
	}
}

// NAME: getVirtualWorld
// INPUT: 2 ints 
// OUTPUT: 1 int 
// DESCRIPTION: Returns the value held in the 2d virtual world array
// at the coordinates (x,y) 
// 0 = empty
// 1 = earth
// 2 = boulder
int StudentWorld::getVirtualWorld(int x, int y)
{
	return virtualWorld[x][y];
}

// NAME: annoyProtestors
// INPUT: 3 ints 
// OUTPUT: 1 bool (true if successful, false otherwise)
// DESCRIPTION: Attempts to annoy protestors for a total of whatever
// amount is passed in as an input by checking if a protestor is within a
// radius of 3 of the coordinates (x,y)
bool StudentWorld::annoyProtestors(int x, int y, int amount)
{
	vector<Actor*>::iterator it;

	// Checks for and annoys Protestors in a 3x3 radius
	for (it = actorsVector.begin(); it != actorsVector.end(); it++)
	{
		if ((*it)->getX() >= (x - 3) && (*it)->getX() <= (x + 3))
		{
			if ((*it)->getY() >= (y - 3) && (*it)->getY() <= (y + 3))
			{
				// If the object is a protestor and they are alive
				if ((*it)->getProtestor() != 0 && (*it)->getLifeStatus() == true)
				{
					(*it)->annoyed(amount);
					return true;
				}
			}
		}
	}

	return false;
}

// NAME: distanceToTunnelMan
// INPUT: 2 ints 
// OUTPUT: 1 double
// DESCRIPTION: Returns the distance from the object at
// coordinates (x,y) to the tunnelman
double StudentWorld::distanceToTunnelMan(int x, int y)
{
	// Use distance formula
	double newX = (m_tunnelManPointer->getX()) - x;
	double newY = (m_tunnelManPointer->getY()) - y;

	return sqrt((newX * newX) + (newY * newY));
}

// NAME: digEarth
// INPUT: 2 ints 
// OUTPUT: NONE
// DESCRIPTION: Checks for and clears Earth objects
// in a 4x4 radius around the cooordinates (x,y) by setting
// the virtual world 2d array to 0 and deleting the earth 
// pointers in the earth pointers 2d array
void StudentWorld::digEarth(int x, int y)
{
	// Checks for and clears Earth objects in a 4x4 radius
	for (int i = x ; i !=  x + 4; i++)
	{
		int k = y;
		while (k != y + 4)
		{
			virtualWorld[i][k] = 0;
			if (earthPointers[i][k] != nullptr)
			{
				playSound(SOUND_DIG);

				delete earthPointers[i][k];
				earthPointers[i][k] = nullptr;
			}
			k++;
		}
	}
}

// NAME: cleanUp
// INPUT: NONE
// OUTPUT: NONE
// DESCRIPTION: Deletes all of the actors from the 
// game by iterating through the actors vector, deletes all
// of the earth pointers , sets the virtual world 2d array to 
// empty, and deletes the tunnelman (destructor uses this function)
void StudentWorld::cleanUp()
{
	vector<Actor*>::iterator it;

	// Delete every actor in the actors vector
	it = actorsVector.begin();
	while (it != actorsVector.end())
	{
		delete *it;
		it = actorsVector.erase(it);
	}

	// Delete every pointer in the earth pointers array
	for (int x = 0; x != 64; x++)
	{
		int y = 0;
		while (y != 60)
		{
			virtualWorld[x][y] = 0;
			if (earthPointers[x][y] != nullptr)
			{
				delete earthPointers[x][y];
				earthPointers[x][y] = nullptr;
			}
			y++;
		}
	}

	// Delete the tunnelman
	delete m_tunnelManPointer;
	m_tunnelManPointer = nullptr;
}
