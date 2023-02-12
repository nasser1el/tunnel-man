#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
#include <queue>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR BASE CLASS IMPLEMENTATIONS //

Actor::Actor(int imageID, int x, int y, StudentWorld* studentWorld, Direction direction, float size, unsigned int depth)
	: GraphObject(imageID, x, y, direction, size, depth)
{
	studentWorldPointer = studentWorld;
	setVisible(true);
	m_lifeStatus = true;
	m_hitPoints = 0;
	m_protestor = 0;
}

Actor::~Actor()
{
}

// NAME: annoyed
// INPUT: 1 int
// OUTPUT: NONE
// DESCRIPTION: Used only for the tunnelman and protestors but implemented as
// a virtual function in actors so I could use it in the StudentWorld class
void Actor::annoyed(int amount)
{
}

// NAME: bribes
// INPUT: NONE
// OUTPUT: NONE
// DESCRIPTION: Used only for protestors but implemented as
// a virtual function in actors so I could use it in the StudentWorld class
void Actor::bribed()
{
}

// NAME: kill
// INPUT: NONE
// OUTPUT: NONE
// DESCRIPTION: Sets the life status of the current actor to false (dead) so
// that it doesn't perform any of its actions during the ticks
void Actor::kill()
{
	m_lifeStatus = false;
}

// NAME: setProtestor
// INPUT: 1 int 
// OUTPUT: NONE
// DESCRIPTION: The actor class has a private variable m_protestor that I 
// used to keep track of which actors were protestors. By default, this variable
// was set to 0 (not a protestor) but the RegularProtestor class sets it to 1 and
// the HardcoreProtestor class sets it to 2
void Actor::setProtestor(int status)
{
	// 0 = not a protestor
	// 1 = regular protestor
	// 2 = hardcore protestor
	m_protestor = status;
}

// NAME: getWorld
// INPUT: NONE
// OUTPUT: NONE
// DESCRIPTION: returns a pointer to the current StudentWorld (allows
// me to use StudentWorld functions like getLevel() or getTunnelMan())
StudentWorld* Actor::getWorld()
{
	return studentWorldPointer;
}

// NAME: getLifeStatus
// INPUT: NONE
// OUTPUT: 1 bool
// DESCRIPTION: returns an actor's life status
bool Actor::getLifeStatus()
{
	return m_lifeStatus;
}

// NAME: getHitPoints
// INPUT: NONE
// OUTPUT: 1 int
// DESCRIPTION: returns an actor's hitpoints (health)
int Actor::getHitPoints()
{
	return m_hitPoints;
}

// NAME: setHitPoints
// INPUT: 1 int
// OUTPUT: NONE
// DESCRIPTION: sets an actor's hitpoints (health) to the input
void Actor::setHitPoints(int amount)
{
	m_hitPoints = amount;
}

// NAME: setLifeStatus
// INPUT: 1 bool
// OUTPUT: NONE
// DESCRIPTION: sets an actor's life status to the input (true/false)
void Actor::setLifeStatus(bool status)
{
	m_lifeStatus = status;
}

// NAME: getProtestor
// INPUT: NONE
// OUTPUT: 1 int
// DESCRIPTION: returns an actor's m_protestor value (tells us whether 
// the actor is not a protestor, a regular protestor, or a hardcore protestor)
int Actor::getProtestor()
{
	return m_protestor; 
}

// EARTH IMPLEMENTATIONS //

// When it is first created:
// 1. A Earth object must have an image ID of TID_EARTH.
// 2. Each Earth object must have its x, y location specified for it –the StudentWorld class can pass in this x, y location when 
// constructing a new Earth object (e.g., when constructing the entire oil field).
// 3. Each Earth object must start out facing rightward.
// 4. Earth has the following graphic parameters: 
	// a. It has an image depth of 3 – meaning its graphic image should always be in the background (all other game objects have smaller depth values)
	// b. It has a size of .25, meaning it occupies only a 1x1 square in the oil field.
// pg. 30

// DONE: 1, 2, 3, 4
// TODO: 
Earth::Earth(int x, int y, StudentWorld* studentWorld)
	: Actor(TID_EARTH, x, y, studentWorld, right, 0.25, 3)
{
	setVisible(true);
}

Earth::~Earth()
{
}

// NAME: doSomething 
// INPUT: NONE
// OUTPUT: NONE
// DESCRIPTION: It’s earth–what do you expect it to do? It does nothing! As such, it doesn't need to have a doSomething() method... 
// Or if it does have one, it doesn't need to do anything. (from specs)
void Earth::doSomething()
{
	return;
}

// TUNNELMAN IMPLEMENTATIONS //

// When it is first created:
// 1. The TunnelMan must have an image ID of TID_PLAYER.
// 2. The TunnelManmust always start at location x=30, y=60.
// 3.The TunnelMan, in its default state: 
	// a. Is given 10 hit points
	// b. Is given 5 units of water to squirt with his squirt gun (he may pick up additional Water in the oil field)
	// c. Is given 1 sonar charge. 
	// d. Starts out with zero gold nuggets
	// e. Should start facing rightward 
// 4. The TunnelMan has the following graphic parameters:
	// a. It has an image depth of 0 –meaning its graphic image should always be in the foreground (above other images)
	// b. It has a size of 1.0
// pg. 27

// DONE: 1, 2, 3, 4
// TODO: 
TunnelMan::TunnelMan(StudentWorld* studentWorld)
	: Actor(TID_PLAYER, 30, 60, studentWorld, right, 1.0, 0)
{
	setHitPoints(10);
	m_squirts = 5;
	m_sonarCharges = 1;
	m_goldNuggets = 0;
	m_barrelsOfOil = 0;
	m_barrelsOfOilToPickUp = getWorld()->getBarrelsOfOilInField();
	setVisible(true);
}

TunnelMan::~TunnelMan()
{
}

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

// DONE: 1, 3a, 3b, 3c, 3d, 3e, 3f, 2
// TODO: 
void TunnelMan::doSomething()
{
	// check if tunnelman is currently alive
	if (getLifeStatus() == false)
		return;

	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		// user hit a key this tick! 
		switch (ch)
		{
		case KEY_PRESS_ESCAPE:
			kill();
			break;
		case KEY_PRESS_SPACE:

			// Get direction the tunnelman is facing in
			if (getDirection() == left)
			{
				// If the squirt would not go out of bounds immediately, squirt in that direction
				// repeat and check for all other directions
				if (getX() > 0)
					getWorld()->squirt(getX() - 1, getY(), left);
			}
			else if (getDirection() == right)
			{
				if (getX() < 60)
					getWorld()->squirt(getX() + 1, getY(), right);
			}
			else if (getDirection() == down)
			{
				if (getY() > 0)
					getWorld()->squirt(getX(), getY() - 1, down);
			}
			else if (getDirection() == up)
			{
				if (getY() < 60)
					getWorld()->squirt(getX(), getY() + 1, down);
			}
			break;

		case KEY_PRESS_LEFT:
			// If the player wants to go left and is facing left... 
			if (getDirection() == left)
			{
				// Check if the left space has a boulder or is out of bounds and then move there
				// and then dig the earth in the tunnelman's new position
				if (getX() > 0 && getWorld()->isBoulder(getX() - 1, getY()) == false)
				{
					moveTo(getX() - 1, getY());
					getWorld()->digEarth(getX(), getY());
				}
			}
			// Othewise, make the tunnelman face left
			// Repeat same process for all other directions
			else
				setDirection(left);
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() == right)
			{
				if (getX() < 60 && getWorld()->isBoulder(getX() + 1, getY()) == false)
				{
					moveTo(getX() + 1, getY());
					getWorld()->digEarth(getX(), getY());
				}
			}
			else
				setDirection(right);
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() == down)
			{
				if (getY() > 0 && getWorld()->isBoulder(getX(), getY() - 1) == false)
				{
					moveTo(getX(), getY() - 1);
					getWorld()->digEarth(getX(), getY());
				}
			}
			else
				setDirection(down);
			break;
		case KEY_PRESS_UP:
			if (getDirection() == up)
			{
				if (getY() < 60 && getWorld()->isBoulder(getX(), getY() + 1) == false)
				{
					moveTo(getX(), getY() + 1);
					getWorld()->digEarth(getX(), getY());
				}
			}
			else
				setDirection(up);
			break;
		case KEY_PRESS_TAB:
			// If the player wants to drop a gold nugget...
			// Check if they have at least 1 gold nugget
			if (m_goldNuggets > 0)
			{
				// Then place a gold nugget at the current location
				// and decrement the number of gold nuggets the tunnelman has
				getWorld()->placeGoldNugget(getX(), getY(), 1);
				setGold(m_goldNuggets - 1);
			}
			break;
		case 'Z':
		case 'z':
			// If the player presses Z or z and wants to use a sonar charge...
			// Check if they have at least 1 sonar charge
			if (m_sonarCharges > 0)
			{
				// Then use the sonar charge at their current location
				// and decrement the number of sonar charges the tunnelman has
				getWorld()->useSonarCharge(getX(), getY());
				setSonarCharges(m_sonarCharges - 1);
			}
			break;
		}
	}
}

// NAME: annoyed
// INPUT: 1 int
// OUTPUT: NONE
// DESCRIPTION: Reduces the tunnelman's hitpoints (health) by the amount passed
// in by the function (however much the tunnelman was annoyed by) and then checks
// if the tunnelman's hitpoints have fallen to or below 0 and, if so, sets his life
// status to dead (false)
void TunnelMan::annoyed(int amount)
{
	setHitPoints(getHitPoints() - amount);

	// if the tunnelman died after being annoyed, set his life 
	// status to dead (false)
	if (getHitPoints() <= 0)
		setLifeStatus(false);
}

// NAME: setSquirts
// INPUT: 1 int
// OUTPUT: NONE
// DESCRIPTION: Sets the tunnelman's squirts to the amount passed
// in by the function
void TunnelMan::setSquirts(int amount)
{
	m_squirts = amount;
}

// NAME: setGold
// INPUT: 1 int
// OUTPUT: NONE
// DESCRIPTION: Sets the tunnelman's gold count to the amount passed
// in by the function
void TunnelMan::setGold(int amount)
{
	m_goldNuggets = amount;
}

// NAME: setSonarCharges
// INPUT: 1 int
// OUTPUT: NONE
// DESCRIPTION: Sets the tunnelman's sonar charge count to the amount passed
// in by the function
void TunnelMan::setSonarCharges(int amount)
{
	m_sonarCharges = amount;
}

// NAME: setBarrels
// INPUT: 1 int
// OUTPUT: NONE
// DESCRIPTION: Sets the tunnelman's barrel count to the amount passed
// in by the function
void TunnelMan::setBarrels(int amount)
{
	m_barrelsOfOil = amount;
}

// NAME: getSquirts
// INPUT: NONE
// OUTPUT: 1 int
// DESCRIPTION: returns the tunnelman's squirt count
int TunnelMan::getSquirtsLeftInSquirtGun()
{
	return m_squirts;
}

// NAME: getPlayerGoldCount
// INPUT: NONE
// OUTPUT: 1 int
// DESCRIPTION: returns the tunnelman's gold count
int TunnelMan::getPlayerGoldCount()
{
	return m_goldNuggets;
}

// NAME: getPlayerSonarChargeCount
// INPUT: NONE
// OUTPUT: 1 int
// DESCRIPTION: returns the tunnelman's sonar charge count
int TunnelMan::getPlayerSonarChargeCount()
{
	return m_sonarCharges;
}

// NAME: getNumberOfBarrels
// INPUT: NONE
// OUTPUT: 1 int
// DESCRIPTION: returns the tunnelman's barrel count
int TunnelMan::getNumberOfBarrels()
{
	return m_barrelsOfOil;
}

// NAME: getNumberOfBarrelsRemainingToBePickedUp()
// INPUT: NONE
// OUTPUT: 1 int
// DESCRIPTION: returns the number of barrels the tunnelman
// still needs to pick up to end the level
int TunnelMan::getNumberOfBarrelsRemainingToBePickedUp()
{
	return (m_barrelsOfOilToPickUp - m_barrelsOfOil);
}

// SQUIRT IMPLEMENTATIONS // 

// When it is first created:
// 1. The Squirt object must have an image ID of TID_WATER_SPURT. 
// 2. All Squirt objects must have their x, y location specified for them by the TunnelMan.
// 3. All Squirt objects must have their direction specified for them by the TunnelMan. 
// 4. All Squirt objects start off with an initial travel distance of 4 squares
// 5. All Squirt objects have the following graphic parameters: 
	// a. They have an image depth of 1 –behind actors like Protesters, but above Earth
	// b. They have a size of 1.0
// pg. 32/33

// DONE: 1, 2, 3, 4, 5
// TODO: 
Squirt::Squirt(int x, int y, Direction direction, StudentWorld* studentWorld)
	: Actor(TID_WATER_SPURT, x, y, studentWorld, direction, 1.0, 1) 
{
	m_maxTravelDistance = 4;
	m_travelDistance = 0;
}

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

// DONE: 1, 2, 3, 4
// TODO: 
void Squirt::doSomething()
{

	// Check if the squirt has traveled the max distance
	if (m_travelDistance >= m_maxTravelDistance)
		kill();

	if (getWorld()->annoyProtestors(getX(), getY(), 2) == true)
		kill();

	if (getDirection() == left)
	{
		// Check if the squirt goes out of bounds
		if (getX() - 1 < 0)
			kill();
	
		// Check if there is a Boulder or Earth object where the squirt is going
		else if (getWorld()->getVirtualWorld(getX(), getY() + 1) != 0)
			kill();

		// All kill conditions have been checked so the squirt can move forward
		else
		{
			m_travelDistance++;
			moveTo(getX() - 1, getY());
		}
	}
	else if (getDirection() == right)
	{
		if (getX() + 1 > 60)
			kill();
		else if (getWorld()->getVirtualWorld(getX(), getY() + 1) != 0)
			kill();
		else
		{
			m_travelDistance++;
			moveTo(getX() + 1, getY());
		}

	}
	else if (getDirection() == down)
	{
		if (getY() - 1 < 0)
			kill();
		else if (getWorld()->getVirtualWorld(getX(), getY() + 1) != 0)
			kill();
		else
		{
			m_travelDistance++;
			moveTo(getX(), getY() - 1);
		}
	}
	else if (getDirection() == up)
	{
		if (getY() + 1 > 60)
			kill();
		else if (getWorld()->getVirtualWorld(getX(), getY() + 1) != 0)
			kill();
		else
		{
			m_travelDistance++;
			moveTo(getX(), getY() + 1);
		}
	}


	return;
}


// BOULDER IMPLEMENTATIONS //

// When it is first created:
// 1. The Boulder object must have an image ID of TID_BOULDER.
// 2. A Boulder object must have its x,y location specified for it –the StudentWorld class can pass in this x,y location when constructing a new Boulder object.
// 3. Boulders start out in a stable state (more on this below).
// 4. Boulders start out facing downward.
// 5. Boulders have the following graphic parameters:
	// a. They have an image depth of 1 –behind actors like Protesters, but above Earth
	// b. They have a size of 1.0
// pg. 31

// DONE: 1, 2, 3, 4, 5
// TODO: 
Boulder::Boulder(int x, int y, StudentWorld* studentWorld)
	: Actor(TID_BOULDER, x, y, studentWorld, down, 1.0, 1) 
{
	setStateToStable();
	m_ticks = 0;
}

// NAME: setStateToStable
// INPUT: NONE
// OUTPUT: NONE
// DESCRIPTION: sets m_state (used to track the boulder's state)
// to 1 (stable)
void Boulder::setStateToStable()
{
	m_state = 1;
}

// NAME: setStateToWaiting
// INPUT: NONE
// OUTPUT: NONE
// DESCRIPTION: sets m_state (used to track the boulder's state)
// to 2 (waiting)
void Boulder::setStateToWaiting()
{
	m_state = 2;
}

// NAME: setStateToFalling
// INPUT: NONE
// OUTPUT: NONE
// DESCRIPTION: sets m_state (used to track the boulder's state)
// to 3 (falling)
void Boulder::setStateToFalling()
{
	m_state = 3;
}

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

// DONE: 1, 2, 3, 4a, 4b
// TODO: 
void Boulder::doSomething()
{
	if (getLifeStatus() == false)
		return;

	// 1 = stable
	// 2 = waiting
	// 3 = falling

	if (m_state == 1)
	{
		for (int x = getX(); x != (getX() + 4); x++)
		{
			int y = getY() - 1;
			if (getWorld()->getVirtualWorld(x, y) == 1)
				return;
		}

		setStateToWaiting();
		m_ticks = 0;
	}

	if (m_state == 2)
	{
		if (m_ticks == 30)
		{
			setStateToFalling();
			// Clear the area where the boulder was so the tunnelman can walk through it
			getWorld()->setEmpty(getX(), getY());
			getWorld()->playSound(SOUND_FALLING_ROCK);
		}
		else
		{
			m_ticks++;
			return;
		}
	}

	if (m_state == 3)
	{
		int x = getX();
		for (int y = getY() - 1; y > -1; y--)
		{
			if (getWorld()->getVirtualWorld(x, y) != 0)
			{
				kill();
				return;
			}

			moveTo(x, y);

			// ANNOY PROTESTORS 
			getWorld()->annoyProtestors(getX(), getY(), 100);

			// KILL TUNNELMAN IF HE'S RAN INTO
			if (getWorld()->distanceToTunnelMan(getX(), getY()) <= 3)
			{
				getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
				getWorld()->getTunnelMan()->annoyed(100);
			}
		}

		kill();
		return;
	}
}


// GOODIE BASE CLASS IMPLEMENTATIONS //

Goodie::Goodie(int imageID, int x, int y, Direction direction, StudentWorld* studentWorld, float size, unsigned int depth)
	: Actor(imageID, x, y, studentWorld, direction, size, depth) 
{
	m_ticks = 0;
}

// NAME: setTicks
// INPUT: 1 int
// OUTPUT: NONE
// DESCRIPTION: sets m_ticks to the value passed in by
// the function
void Goodie::setTicks(int numberOfTicks)
{
	m_ticks = numberOfTicks;
}

// NAME: getTicks
// INPUT: NONE
// OUTPUT: 1 int
// DESCRIPTION: returns the number of ticks 
int Goodie::getTicks()
{
	return m_ticks;
}

// BARRELOFOIL IMPLEMENTATIONS // 

// When it is first created:
// 1. All Barrels must have an image ID of TID_BARREL.
// 2. All Barrels must have their x, y location specified for them when they are created.
// 3. All Barrels must start off facing rightward.
// 4. All Barrels start out invisible and they must be discovered (by the TunnelMan walking in close proximity to them) before they become visible.
// 5. Barrels have the following graphic parameters: 
	// a. They have an image depth of 2 –behind actors like Protesters, but above Earth
	// b. They have a size of 1.0
// pg. 33/34

// DONE: 1, 2, 3, 4, 5
// TODO: 
BarrelOfOil::BarrelOfOil(int x, int y, StudentWorld* studentWorld)
	: Goodie(TID_BARREL, x, y, right, studentWorld, 1.0, 2) 
{
	setVisible(false);
}

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

// DONE: 1, 2, 3a, 3b, 3c, 3d
// TODO: 
void BarrelOfOil::doSomething()
{
	// Check if the barrel is alive and, if not, return
	if (getLifeStatus() == false)
		return; 

	// Check if the barrel is invisible and within a radius of 4.00 from the TunnelMan
	else if (isVisible() == false && getWorld()->distanceToTunnelMan(getX(), getY()) <= 4.00)
	{
		// Make it visible and return
		setVisible(true);
		return;
	}

	// Check if the barrel is within a radius of 3.00 from the TunnelMan
	else if (getWorld()->distanceToTunnelMan(getX(), getY()) <= 3.00)
	{
		// Kill it, play the sound effect, increment the tunnelman's number
		// of barrels and increase the player's score 
		kill();
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->getTunnelMan()->setBarrels(getWorld()->getTunnelMan()->getNumberOfBarrels() + 1);
		getWorld()->increaseScore(1000);
	}
}

// GOLDNUGGET IMPLEMENTATIONS // 

// When it is first created:
// 1. All Gold Nuggets must have an image ID of TID_GOLD.
// 2. All Gold Nuggets must have their x,y location specified for them when they are created.
// 3. All Gold Nuggets must start off facing rightward.
// 4. A Gold Nugget may either start out invisible or visible –this must be specified by the code that creates the Nugget, depending on the context of its creation. 
// Nuggets buried within the Earth of the oil field always start out invisible, whereas Nuggets dropped by the TunnelMan start out visible.
// 5. A Gold Nugget will either be pickup-able by the TunnelMan or pickup-able by Protesters, but not both. 
// This state must be specified by the code that creates the Gold Nugget object.
// 6. A Gold Nugget will either start out in a permanent state (where they will remain in the oil field until they are picked up by the TunnelMan or the level ends) 
// or a temporary state (where they will only remain in the oil field for a limited number of ticks before disappearing or being picked up by a Protester). 
// This state must be specified by the code that creates the Gold Nugget object.
// 7. Gold Nuggets have the following graphic parameters:
	// a. They have an image depth of 2 –behind actors like Protesters, but above Earth
	// b. They have a size of 1.0
// pg. 35

// DONE: 1, 2, 3, 4, 5, 6, 7
// TODO:
GoldNugget::GoldNugget(int x, int y, StudentWorld* studentWorld, int state)
	: Goodie(TID_GOLD, x, y, right, studentWorld, 1.0, 2)
{
	m_state = state;
	setTicks(0);
	setVisible(false);
}

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

// DONE: 1, 2, 3a, 3b, 3c, 3d, 4a, 4b, 4c, 4d, 5
// TODO:
void GoldNugget::doSomething()
{
	// Check if the gold nugget is alive and, if not, return
	if (getLifeStatus() == false)
		return; 

	// Check if the gold nugget is invisible and within a radius of 4.00 from the TunnelMan
	else if (isVisible() == false && getWorld()->distanceToTunnelMan(getX(), getY()) <= 4.00)
	{
		// Make it visible and return
		setVisible(true);
		return;
	}

	// Check if the gold nugget is within a radius of 3.00 from the TunnelMan and is 
	// in a pickup-able-by-the-tunnelman state
	else if (m_state == 0 && getWorld()->distanceToTunnelMan(getX(), getY()) <= 3.00)
	{
		// Kill it, play the sound effect, increment the player's gold count
		// and increase the player's score 
		kill();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(10);
		getWorld()->getTunnelMan()->setGold(getWorld()->getTunnelMan()->getPlayerGoldCount() + 1);
	}

	// Otherwise, check if the gold nugget is in a pickup-able-by-protestors state
	// (if it was dropped by the tunnelman)
	else if (m_state == 1)
	{
		// If 100 ticks have passed, the dropped gold nugget dies
		if (getTicks() >= 100)
			kill();
		// Otherwise, check if a nearby protestor can pick it up
		else
		{
			// If the gold nugget is picked up by a protestor, it dies
			if (getWorld()->bribeNearbyProtestor(getX(), getY()) == true)
				kill();
			// Otherwise, increment its ticks
			else 
				setTicks(getTicks() + 1);
		}
	}

	return;
}

// NAME: setState
// INPUT: 1 int 
// OUTPUT: NONE
// DESCRIPTION: sets m_state (used to keep track of the gold nugget's state) to
// the input (0 = pickup-able-by-the-tunnelman, 1 = pickup-able-by-protestors) 
void GoldNugget::setState(int state)
{
	m_state = state;
}

// SONARKIT IMPLEMENTATIONS // 

// When it is first created:
// 1. All Sonar Kits must have an image ID of TID_SONAR.
// 2. All Sonar Kits must have their x,y location specified for them when they are created.
// 3. All Sonar Kits must start off facing rightward.
// 4. All Sonar Kits starts out visible.
// 5. A Sonar Kit is only pickup-able by the TunnelMan.
// 6. A Sonar Kit will always start out in a temporary state (where they will only remain in the oil field for a limited number of ticks before disappearing) 
// –the number of ticks T a Sonar Kit will exist can be determined from the following formula: T = max(100, 300 –10 * current_level_number)
// 7. Sonar Kits have the following graphic parameters: 
// a. They have an image depth of 2 –behind actors like Protesters, but aboveEarth
// b. They have a size of 1.0
// pg. 36/37

// DONE: 1, 2, 3, 4, 5, 6, 7
// TODO: 
SonarKit::SonarKit(int x, int y, StudentWorld* studentWorld)
	: Goodie(TID_SONAR, x, y, right, studentWorld, 1.0, 2) 
{
	setVisible(true);
	setTicks(max(100, int(300 - 10 * getWorld()->getLevel())));
}

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

// DONE: 1, 2a, 2b, 2c, 2d, 3
// TODO: 
void SonarKit::doSomething()
{
	// Check if the sonar kit is alive and, if not, return
	if (getLifeStatus() == false)
		return;

	// If all of the ticks have passed, set the sonar kit to dead
	if (getTicks() < 0)
		kill();
	// Otherwise, decrement the number of ticks
	else 
		setTicks(getTicks() - 1);

	// Check if the sonar kit is within a radius of 3.00 from the TunnelMan
	if (getWorld()->distanceToTunnelMan(getX(), getY()) <= 3.00)
	{
		// Kill it, play the sound effect, increment the tunnelman's sonar charge count,
		// and increase the player's score 
		kill();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(75);
		getWorld()->getTunnelMan()->setSonarCharges(getWorld()->getTunnelMan()->getPlayerSonarChargeCount() + 1);
	}
}

// WATERPOOL IMPLEMENTATIONS // 

// When it is first created:
// 1. All Water Pools must have an image ID of TID_WATER_POOL.
// 2. All Water Pools must have their x,y location specified for them when they are created.
// 3. All Water Pools must start off facing rightward.
// 4. All Water Pools start out visible.
// 5. A Water Pool is only pickup-able by the TunnelMan.
// 6. A Water Pool will always start out in a temporary state (where they will only remain in the oil field for a limited number of ticks before disappearing) 
// –the number of ticks T that a Water Poolwill exist can be determined from the following formula: T = max(100, 300 –10*current_level_number)
// 7. Water Pools have the following graphic parameters:
// a. They have an image depth of 2 –behind actors like Protesters, but above Earth
// b. They have a size of 1.0
// pg. 38

// DONE: 1, 2, 3, 4, 5, 6, 7
// TODO: 
WaterPool::WaterPool(int x, int y, StudentWorld* studentWorld)
	: Goodie(TID_WATER_POOL, x, y, right, studentWorld, 1.0, 2) 
{
	setVisible(true);
	setTicks(max(100, int(300 - 10 * getWorld()->getLevel())));
}

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

// DONE: 1, 2a, 2b, 2d, 2c, 3
// TODO: 
void WaterPool::doSomething()
{
	// Check if the water pool is alive and, if not, return
	if (getLifeStatus() == false)
		return;

	// If all of the ticks have passed, set the water pool to dead
	if (getTicks() <= 0)
		kill();
	// Otherwise, decrement the number of ticks
	else
		setTicks(getTicks() - 1);

	// Check if the sonar kit is within a radius of 3.00 from the TunnelMan
	if (getWorld()->distanceToTunnelMan(getX(), getY()) <= 3.00)
	{
		// Kill it, play the sound effect, increase the number of squirts the tunnelman
		// has by 5, and increase the player's score 
		kill();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(100);
		getWorld()->getTunnelMan()->setSquirts(getWorld()->getTunnelMan()->getSquirtsLeftInSquirtGun() + 5);
	}

	return;
}


// PROTESTOR BASE CLASS IMPLEMENTATIONS // 

Protestor::Protestor(int x, int y, StudentWorld* studentWorld, int imageID)
	: Actor(imageID, x, y, studentWorld, left, 1.0, 0) 
{
	setNumSquaresToMoveInCurrentDirection();
	setTicks(0);
	setHitPoints(0);
	setTicksToWaitBetweenMoves(max(0, int(3 - getWorld()->getLevel() / 4)));
	setNonRestingTicks(0);
	setNonRestingPerpindicularTicks(0);
	setLeaveTheOilFieldState(false);
}

// NAME: annoyed
// INPUT: 1 int
// OUTPUT: NONE
// DESCRIPTION: Reduces the protestor's hitpoints (health) by the amount passed
// in by the function (however much the protestor was annoyed by) and then checks
// if the protestor's hitpoints have fallen to or below 0 and, if so, sets their
// state to leave the oil field (true)
void Protestor::annoyed(int amount)
{
	setHitPoints(getHitPoints() - amount);

	// Protestor is stunned
	if (getHitPoints() > 0)
	{
		int N = max(50, int(100 - getWorld()->getLevel() * 10));

		// Delays the protestor from moving for N more ticks 
		setTicks(getTicks() - N);
	}

	// Protestor was hit and killed by a boulder
	else if (getHitPoints() <= 0 && amount == 100 && getLeaveTheOilFieldState() == false)
	{
		// Play the sound effect, put the protestor into a leave-the-oil-field state,
		// and increase the player's score 
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		getWorld()->increaseScore(500);
		setLeaveTheOilFieldState(true);
	}

	// Protestor was hit and killed by a squirt
	else if (getHitPoints() <= 0 && getLeaveTheOilFieldState() == false)
	{
		// Play the sound effect and put the protestor into a leave-the-oil-field state
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		setLeaveTheOilFieldState(true);

		// Increase the player's score appropriately (check if they killed a regular
		// or hardcore protestor)
		// Regular protestor
		if (getProtestor() == 1)
			getWorld()->increaseScore(100);

		// Hardcore protestor
		else
			getWorld()->increaseScore(250);
	}
}

// NAME: setNumSquaresToMoveInCurrentDirection
// INPUT: NONE
// OUTPUT: NONE 
// DESCRIPTION: sets m_numSquaresToMove to a random number between 8 and 60
void Protestor::setNumSquaresToMoveInCurrentDirection() 
{
	m_numSquaresToMove = rand() % 53 + 8;
}

// NAME: bribed
// INPUT: NONE
// OUTPUT: NONE 
// DESCRIPTION: puts the protestor into a leave-the-oil-field-state and
// plays the correct sound
void Protestor::bribed()
{
	setLeaveTheOilFieldState(true);
	getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
}

// NAME: getNumSquaresToMoveInCurrentDirection
// INPUT: NONE
// OUTPUT: 1 int
// DESCRIPTION: returns m_numSquaresToMove
int Protestor::getNumSquaresToMoveInCurrentDirection()
{
	return m_numSquaresToMove;
}

// NAME: getTicksToWaitBetweenMoves
// INPUT: NONE
// OUTPUT: 1 int
// DESCRIPTION: returns m_ticksToWaitBetweenMoves
int Protestor::getTicksToWaitBetweenMoves()
{
	return m_ticksToWaitBetweenMoves;
}

// NAME: getTicks
// INPUT: NONE
// OUTPUT: 1 int
// DESCRIPTION: returns m_ticks
int Protestor::getTicks()
{
	return m_ticks;
}

// NAME: getNonRestingTicks
// INPUT: NONE
// OUTPUT: 1 int
// DESCRIPTION: returns m_nonRestingTicks
int Protestor::getNonRestingTicks()
{
	return m_nonRestingTicks;
}

// NAME: getNonRestingPerpindicularTicks
// INPUT: NONE
// OUTPUT: 1 int
// DESCRIPTION: returns m_nonRestingPerpindicularTicks
int Protestor::getNonRestingPerpindicularTicks()
{
	return m_nonRestingPerpindicularTicks;
}

// NAME: getLeaveTheOilFieldState
// INPUT: NONE
// OUTPUT: 1 bool
// DESCRIPTION: returns m_leaveTheOilFieldState
bool Protestor::getLeaveTheOilFieldState()
{
	return m_leaveTheOilFieldState;
}

// NAME: setNumSquaresToMoveInCurrentDirection
// INPUT: 1 int 
// OUTPUT: NONE
// DESCRIPTION: sets m_numSquaresToMove to the input
void Protestor::setNumSquaresToMoveInCurrentDirection(int number)
{
	m_numSquaresToMove = number;
}

// NAME: setTicks
// INPUT: 1 int 
// OUTPUT: NONE
// DESCRIPTION: sets m_ticks to the input
void Protestor::setTicks(int amount)
{
	m_ticks = amount;
}

// NAME: setNonRestingTicks
// INPUT: 1 int 
// OUTPUT: NONE
// DESCRIPTION: sets m_nonRestingTicks to the input
void Protestor::setNonRestingTicks(int amount)
{
	m_nonRestingTicks = amount;
}

// NAME: setNonRestingPerpindicularTicks
// INPUT: 1 int 
// OUTPUT: NONE
// DESCRIPTION: sets m_nonRestingPerpindicularTicks to the input
void Protestor::setNonRestingPerpindicularTicks(int amount)
{
	m_nonRestingPerpindicularTicks = amount;
}

// NAME: setTicksToWaitBetweenMoves
// INPUT: 1 int 
// OUTPUT: NONE
// DESCRIPTION: sets m_ticksToWaitBetweenMoves to the input
void Protestor::setTicksToWaitBetweenMoves(int amount)
{
	m_ticksToWaitBetweenMoves = amount;
}

// NAME: setLeaveTheOilFieldState
// INPUT: 1 bool
// OUTPUT: NONE
// DESCRIPTION: sets m_leaveTheOilFieldState to the input (true/false)
void Protestor::setLeaveTheOilFieldState(bool state)
{
	m_leaveTheOilFieldState = state;
}

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
int Protestor::protestorQueueMazeSearch(int targetX, int targetY, int exitX, int exitY)
{
	int result = 0;
	// Array to keep track of whether each coordinate has been discovered
	// false = undiscovered
	// true = discovered
	bool discoveredArray[64][64];
	for (int k = 0; k < 64; k++)
	{
		int i = 0;
		while (i < 64)
		{
			discoveredArray[k][i] = false;
			i++;
		}
	}

	int directionArray[64][64];
	for (int k = 0; k < 64; k++)
	{
		int i = 0;
		while (i < 64)
		{
			directionArray[k][i] = 100000;
			i++;
		}
	}


	// insert starting point into queue and mark as discovered
	queue<Point> maze;
	maze.push(Point(exitX, exitY));
	discoveredArray[exitX][exitY] = true;
	directionArray[exitX][exitY] = 0;
	
	Point topPoint = maze.front();
	int topX, topY;
	int stepsCounter = 0;

	// Continuously check if the queue is empty (like recursion)
	while (maze.empty() == false)
	{
		// Process the top point coordinates
		// and then remove the top point from the queue 
		topPoint = maze.front();
		topX = topPoint.getx();
		topY = topPoint.gety();
		maze.pop();
		stepsCounter = directionArray[topX][topY];
		stepsCounter++;

		// Check if the point to the right is not out of bounds, is empty, and has not been discovered
		// If slot to the WEST is open & undiscovered... (repeated for all directions)
		// LEFT = 1
		if (topX - 1 >= 0 && getWorld()->isEmpty(topX - 1, topY) && discoveredArray[topX - 1][topY] == false)
		{
			// Mark (curx - 1, cury) as discovered (repeated for all directions)
			discoveredArray[topX - 1][topY] = true;

			// Insert (topX - 1, topY) on queue (repeated for all directions)
			maze.push(Point(topX - 1, topY));
			directionArray[topX - 1][topY] = stepsCounter;

		}

		// EAST
		// RIGHT = 2
		if (topX + 1 <= 60 && getWorld()->isEmpty(topX + 1, topY) && discoveredArray[topX + 1][topY] == false)
		{
			discoveredArray[topX + 1][topY] = true;
			maze.push(Point(topX + 1, topY));
			directionArray[topX + 1][topY] = stepsCounter;
		}

		// SOUTH
		// DOWN = 3
		if (topY - 1 >= 0 && getWorld()->isEmpty(topX, topY - 1) && discoveredArray[topX][topY - 1] == false)
		{
			discoveredArray[topX][topY - 1] = true;
			maze.push(Point(topX, topY - 1));
			directionArray[topX][topY - 1] = stepsCounter;
		}

		// NORTH
		// UP = 4
		if (topY + 1 <= 60 && getWorld()->isEmpty(topX, topY + 1) && discoveredArray[topX][topY + 1] == false)
		{
			discoveredArray[topX][topY + 1] = true;
			maze.push(Point(topX, topY + 1));
			directionArray[topX][topY + 1] = stepsCounter;
		}

	}

	int shortestPath;

	// Compare all found solutions and return the direction of the
	// solution that requires the least amount of steps
	if (targetX == 0)
	{
		if (targetY == 0)
		{
			shortestPath = min(directionArray[targetX][targetY + 1], directionArray[targetX + 1][targetY]);
			if (shortestPath == directionArray[targetX][targetY + 1])
				result = 4;
			else
				result = 1;
		}
		else
		{
			shortestPath = min(directionArray[exitX + 1][targetY], min(directionArray[targetX][targetY + 1], directionArray[targetX][targetY - 1]));
			if (shortestPath == directionArray[exitX + 1][targetY])
				result = 2;
			else if (shortestPath == directionArray[exitX][exitY - 1])
				result = 3;
			else
				result = 4;
		}
	}
	else if (targetY == 0)
	{
		shortestPath = min(directionArray[targetX][targetY + 1], min(directionArray[targetX - 1][targetY], directionArray[targetX + 1][targetY]));
		if (shortestPath == directionArray[targetX - 1][targetY])
			result = 1;
		else if (shortestPath == directionArray[targetX + 1][targetY])
			result = 2;
		else
			result = 4;
	}
	else
	{
		shortestPath = min(min(directionArray[targetX - 1][targetY], directionArray[targetX + 1][targetY]), min(directionArray[targetX][targetY - 1], directionArray[targetX][targetY + 1]));
		if (shortestPath == directionArray[targetX - 1][targetY])
			result = 1;
		else if (shortestPath == directionArray[targetX + 1][targetY])
			result = 2;
		else if (shortestPath == directionArray[targetX][targetY - 1])
			result = 3;
		else
			result = 4;
	}

	return result;
}



// REGULAR PROTESTOR IMPLEMENTATIONS // 

// When it is first created:
// 1. Each Regular Protester must have an image ID of TID_PROTESTER.
// 2. Each Regular Protester must start out facing left.
// 3. Each Regular Protester must decide how many squares, numSquaresToMoveInCurrentDirection, it will decide to move left before possibly switching its direction. 
// This value must be: 8 <= numSquaresToMoveInCurrentDirection <= 60
// 4. Each Regular Protester starts out with 5 hit-points.
// 5. Each Regular Protester starts out NOT in a leave-the-oil-field state.
// 6. Each Regular Protester have the following graphic parameters:
// a. It has an image depth of 0 –they’re always in the foreground
// b. It has a size of 1.0
// pg. 39

// DONE: 1, 2, 4, 6
// TODO: 3, 5
RegularProtestor::RegularProtestor(int x, int y, StudentWorld* studentWorld)
	: Protestor(x, y, studentWorld, TID_PROTESTER)
{
	setHitPoints(5);
	setProtestor(1);
}

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

// DONE: 1, 2, 4a, 4b, 4c, 4d, 5a, 5b, 5c, 6a, 6b, 6c, 6d, 6e, 7a, 7b, 8, 9
// TODO: 3a, 3b, 3c, 
void RegularProtestor::doSomething()
{
	// Check if the protestor is alive and, if not, return
	if (getLifeStatus() == false)
		return;

	if (getHitPoints() <= 0)
		setLeaveTheOilFieldState(true);

	int direction = 0;
	if (getDirection() == up)
		direction = 1;
	if (getDirection() == down)
		direction = 2;
	if (getDirection() == left)
		direction = 3;
	if (getDirection() == right)
		direction = 4;

	if (getTicks() < getTicksToWaitBetweenMoves())
	{
		// Protestor is in a rest state so update ticks count and return
		setTicks(getTicks() + 1);
		return;
	}

	// 3. Otherwise, if the Regular Protester is in a leave-the-oil-field state (because their hit points reached zero due to being repeatedly squirted by the TunnelMan’s squirt gun or bonked by a falling Boulder), then: 
	// a. If the Regular Protester is at its exit point (at location x = 60, y = 60) then it will immediately set its status to dead so that it will be removed from the game at the end of the current tick (by your StudentWorld class).
	// b. Otherwise, the Regular Protester must move one square closer to its exit point (at x = 60, y = 60). It must determine what direction to move based on a queue-based maze-searching algorithm like the one we learned in class.
	// The Regular Protester may not walk through Earth or Boulders to reach its exit point (meaning that none of the 16 squares that comprise the 4x4 Regular Protester icon may overlap with an Earthobject and that the Regular 
	// Protester must not come within (within means <= ) a distance of 3.0 units of any Boulders).
	// Hint: It’s inefficient for every single RegularProtester to try to figure out its own optimal path to the exit, so see if you can figure out a way to compute a single data structure that enables all Protesters to 
	// quickly determine their optimal path to their common exitpoint of 60, 60, no matter where they are located in the oil field.
	// c. After moving one step toward their exit point, the Regular Protester will return immediately.
	// It will not perform any of the following steps.

	if (getLeaveTheOilFieldState() == true)
	{
		setTicks(0);
		setNonRestingTicks(getNonRestingTicks() + 1);
		setNonRestingPerpindicularTicks(getNonRestingPerpindicularTicks() + 1);

		int finalDirection = 0; 
		if (getX() == 60 && getY() == 60)
		{
			kill();
			return;
		}
		else if (protestorQueueMazeSearch(getX(), getY(), 60, 60) == 1)
		{
			if (getWorld()->isEmpty(getX() - 1, getY()) == true)
			{
				setDirection(left);
				moveTo(getX() - 1, getY());
				return;
			}
		}
		else if (protestorQueueMazeSearch(getX(), getY(), 60, 60) == 2)
		{
			if (getWorld()->isEmpty(getX() + 1, getY()) == true)
			{
				setDirection(right);
				moveTo(getX() + 1, getY());
				return;
			}
		}

		else if (protestorQueueMazeSearch(getX(), getY(), 60, 60) == 3)
		{
			if (getWorld()->isEmpty(getX(), getY() - 1) == true)
			{
				setDirection(down);
				moveTo(getX(), getY() - 1);
				return;
			}
		}

		else if (protestorQueueMazeSearch(getX(), getY(), 60, 60) == 4)
		{
			if (getWorld()->isEmpty(getX(), getY() + 1) == true)
			{
				setDirection(up);
				moveTo(getX(), getY() + 1);
				return;
			}
		}

		return;
	}

	// 4. Otherwise, the Regular Protester will check to see if they are within a distance of 4 units of the TunnelMan, AND they are currently facing in the TunnelMan’s direction. 
	// If both are true and the Regular Protester hasn’t shouted within its last non-resting 15 ticks, then the Regular Protester will: 
	// a. Play the shouting sound: SOUND_PROTESTER_YELL
	// b. Inform the TunnelMan that he’s been annoyed for a total of 2 annoyance points (deducting 2 points from the TunnelMan’s hit points, and possibly causing the TunnelMan to become sufficiently annoyed to quit the level (aka die)).
	// c. Update some state variable in a manner that prevents this RegularProtester from shouting again for at least 15 non-resting ticks.
	// d. Return immediately.
	else if (getWorld()->distanceToTunnelMan(getX(), getY()) <= 4.00) // Check if the protestor is facing TunnelMan's direction
	{
		// Reset the ticks counter
		// Increment the non-resting ticks counter
		setTicks(0);
		setNonRestingTicks(getNonRestingTicks() + 1);
		setNonRestingPerpindicularTicks(getNonRestingPerpindicularTicks() + 1);


		// Each if statement checks if the protestor is facing the same direction as the tunnelman
		if (getDirection() == up)
		{
			if (getX() < getWorld()->getTunnelMan()->getX() + 4 && getX() > getWorld()->getTunnelMan()->getX() - 4
				&& getY() >= getWorld()->getTunnelMan()->getY() - 4 && getY() <= getWorld()->getTunnelMan()->getY())
			{
				// Check if 15 non-resting ticks have passed and, if so, shout
				if (getNonRestingTicks() >= 15)
				{
					// Reset the non-resting ticks counter 
					setNonRestingTicks(0);
					getWorld()->playSound(SOUND_PROTESTER_YELL);
					// ANNOY TUNNELMAN
					getWorld()->getTunnelMan()->annoyed(2);
				}
			}
		}

		else if (getDirection() == down)
		{
			if (getX() < getWorld()->getTunnelMan()->getX() + 4 && getX() > getWorld()->getTunnelMan()->getX() - 4
				&& getY() >= getWorld()->getTunnelMan()->getY() && getY() <= getWorld()->getTunnelMan()->getY() + 4)
			{
				// Check if 15 non-resting ticks have passed and, if so, shout
				if (getNonRestingTicks() >= 15)
				{
					// Reset the non-resting ticks counter 
					setNonRestingTicks(0);
					getWorld()->playSound(SOUND_PROTESTER_YELL);
					// ANNOY TUNNELMAN
					getWorld()->getTunnelMan()->annoyed(2);
				}
			}
		}

		else if (getDirection() == right)
		{
			if (getX() <= getWorld()->getTunnelMan()->getX() && getX() >= getWorld()->getTunnelMan()->getX() - 4
				&& getY() > getWorld()->getTunnelMan()->getY() - 4 && getY() < getWorld()->getTunnelMan()->getY() + 4)
			{
				// Check if 15 non-resting ticks have passed and, if so, shout
				if (getNonRestingTicks() >= 15)
				{
					// Reset the non-resting ticks counter 
					setNonRestingTicks(0);
					getWorld()->playSound(SOUND_PROTESTER_YELL);
					// ANNOY TUNNELMAN
					getWorld()->getTunnelMan()->annoyed(2);
				}
			}
		}

		else if (getDirection() == left)
		{
			if (getX() <= getWorld()->getTunnelMan()->getX() + 4 && getX() >= getWorld()->getTunnelMan()->getX()
				&& getY() > getWorld()->getTunnelMan()->getY() - 4 && getY() < getWorld()->getTunnelMan()->getY() + 4)
			{
				// Check if 15 non-resting ticks have passed and, if so, shout
				if (getNonRestingTicks() >= 15)
				{
					// Reset the non-resting ticks counter 
					setNonRestingTicks(0);
					getWorld()->playSound(SOUND_PROTESTER_YELL);
					// ANNOY TUNNELMAN
					getWorld()->getTunnelMan()->annoyed(2);
				}
			}
		}

		return; 
	}
	// 5. Otherwise, if the Regular Protester:
	// a. Is in a straight horizontal or vertical line of sight to the TunnelMan(even if the Regular Protester isn’t currently facing the TunnelMan), and 
	// b. Is more than 4 units away from the TunnelMan –that is, the radius from the Regular Protester and theTunnelMan is greater than 4.0 units away, and
	// c. Could actually move the entire way to the TunnelMan with no Earth or Boulders blocking its path (assuming it kept walking straight over the next N turns) 
	// then the Regular Protester will:
		// a. Change its direction to face in the direction of the TunnelMan, AND then take one step toward him.
		// b. The Regular Protester will set its numSquaresToMoveInCurrentDirection value to zero, forcing it to pick a new direction/distance to move during 
		// its next non-resting tick (unless of course, the Regular Protester still sees the TunnelMan in its line of sight, in which case it will continue to move toward the TunnelMan).
		// c. Then the Regular Protester will immediately return.
	else if (getWorld()->distanceToTunnelMan(getX(), getY()) > 4.00 && (getWorld()->getTunnelMan()->getX() == getX() || getWorld()->getTunnelMan()->getY() == getY()))
	{
		// Reset the ticks counter
		// Increment the non-resting ticks counter
		setTicks(0);
		setNonRestingTicks(getNonRestingTicks() + 1);
		setNonRestingPerpindicularTicks(getNonRestingPerpindicularTicks() + 1);

		// Make sure the path to the tunnelman is clear
		if (getWorld()->isPathClear(getX(), getY(), 1) == true)
		{
			// Set direction towards the tunnelman
			setDirection(up);
			
			// Move up if possible
			if (getY() + 1 <= 60 && getWorld()->isEmpty(getX(), getY() + 1) == true)
				moveTo(getX(), getY() + 1);
		}
		else if (getWorld()->isPathClear(getX(), getY(), 2) == true)
		{
			setDirection(down);
			if (getY() - 1 >= 0 && getWorld()->isEmpty(getX(), getY() - 1) == true)
				moveTo(getX(), getY() - 1);
		}
		else if (getWorld()->isPathClear(getX(), getY(), 3) == true)
		{
			setDirection(left);
			if (getX() - 1 >= 0 && getWorld()->isEmpty(getX() - 1, getY()) == true)
				moveTo(getX() - 1, getY());
		}
		else if (getWorld()->isPathClear(getX(), getY(), 4) == true)
		{
			setDirection(right);
			if (getX() + 1 <= 60 && getWorld()->isEmpty(getX() + 1, getY()) == true)
				moveTo(getX() + 1, getY());
		}

		// set numSquaresToMoveInCurrentDirection to 0
		setNumSquaresToMoveInCurrentDirection(0);
		return;
	}
	// 6. Otherwise, the Regular Protester can’t directly see the TunnelMan. As such, it will decrement its numSquaresToMoveInCurrentDirection variable by one. 
	else
	{
		// Reset the ticks counter
		// Increment the non-resting ticks counter
		setTicks(0);
		setNonRestingTicks(getNonRestingTicks() + 1);
		setNonRestingPerpindicularTicks(getNonRestingPerpindicularTicks() + 1);
		setNumSquaresToMoveInCurrentDirection(getNumSquaresToMoveInCurrentDirection() - 1);
	}
	// If the Regular Protester has finished walking numSquaresToMoveInCurrentDirection steps in its currently-selected direction (i.e.,numSquaresToMoveInCurrentDirection <= 0), then:
	// a. The Regular Protester will pick a random new direction to move (up, down, left or right).
	// b. If the random direction is blocked either by Earth or a Boulder such that it can’t take even a single step in that chosen direction, then it will select a different direction and check it for
	// blockage (it will continue checking directions until it has picked a direction that is not blocked).
	// c. The Regular Protester will then change its direction to this new chosen direction.
	// d. The Regular Protester will then pick a new value for numSquaresToMoveInCurrentDirection that will govern how far it should move in the selected direction over the next numSquaresToMoveInCurrentDirection non-resting ticks.
	// The value must be such that: 8 <= numSquaresToMoveInCurrentDirection<= 60.
	// e. The Regular Protester will then continue with step 8.
	if (getNumSquaresToMoveInCurrentDirection() <= 0)
	{
		// Once getNumSquaresToMoveInCurrentDirection has reached zero...
		for (;;)
		{
			// Pick a new random direction and check if that direction is viable
			// Loop until a viable direction is found
			int randomDirection = rand() % 4 + 1;
			if (randomDirection == 1)
			{
				if (getWorld()->isEmpty(getX(), getY() + 1) == true)
				{
					setDirection(up);
					break;
				}
			}
			if (randomDirection == 2)
			{
				if (getWorld()->isEmpty(getX(), getY() - 1) == true)
				{
					setDirection(down);
					break;
				}
			}
			if (randomDirection == 3)
			{
				if (getWorld()->isEmpty(getX() - 1, getY()) == true)
				{
					setDirection(left);
					break;
				}
			}
			if (randomDirection == 4)
			{
				if (getWorld()->isEmpty(getX() + 1, getY()) == true)
				{
					setDirection(right);
					break;
				}
			}
		}

		// Set a new numSquaresToMoveInCurrentDirection
		setNumSquaresToMoveInCurrentDirection();
	}
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
	else if (getNonRestingPerpindicularTicks() >= 200)
	{
		// Remember that it made a perpindicular turn by setting nonRestingPerpindicularTicks to 0 
		setNonRestingPerpindicularTicks(0);

		// If the Protestor is facing left or right, check up and down
		if (getDirection() == left || getDirection() == right)
		{
			// If only up is a viable path, set direction to up
			if (getWorld()->isEmpty(getX(), getY() + 1) == true && getWorld()->isEmpty(getX(), getY() - 1) == false)
				setDirection(up);
			// if only down is a viable path, set direction to down
			else if (getWorld()->isEmpty(getX(), getY() + 1) == false && getWorld()->isEmpty(getX(), getY() - 1) == true)
				setDirection(down);
			// if both paths are viable, create a random number from 1-100 and, if it is less than 50, go up
			// Otherwise, go down
			else if (getWorld()->isEmpty(getX(), getY() + 1) == true && getWorld()->isEmpty(getX(), getY() - 1) == true)
			{
				int randomWay = rand() % 100 + 1;
				if (randomWay < 50)
					setDirection(up);
				else 
					setDirection(down);
			}
			// If the direction didn't change then neither of the paths were viable so we pick a new random numSquaresToMoveInCurrentDirection
			if (getDirection() == up || getDirection() == down)
			{
				setNumSquaresToMoveInCurrentDirection();
				setNonRestingPerpindicularTicks(0);
			}
		}
		// If the Protestor is facing up or down, check left and right
		else if (getDirection() == up || getDirection() == down)
		{
			// If only right is a viable path, set direction to right
			if (getWorld()->isEmpty(getX() + 1, getY()) == true && getWorld()->isEmpty(getX() - 1, getY()) == false)
				setDirection(right);
			// if only left is a viable path, set direction to left
			else if (getWorld()->isEmpty(getX() + 1, getY()) == false && getWorld()->isEmpty(getX() - 1, getY()) == true)
				setDirection(left);
			// if both paths are viable, create a random number from 1-100 and, if it is less than 50, go right
			// Otherwise, go left
			else if (getWorld()->isEmpty(getX() + 1, getY()) == true && getWorld()->isEmpty(getX() - 1, getY()) == true)
			{
				int randomWay = rand() % 100 + 1;
				if (randomWay < 50)
					setDirection(right);
				else
					setDirection(left);
			}
			// If the direction didn't change then neither of the paths were viable so we pick a new random numSquaresToMoveInCurrentDirection
			if (getDirection() == left || getDirection() == right)
			{
				setNumSquaresToMoveInCurrentDirection();
				setNonRestingPerpindicularTicks(0);
			}
		}
	}

	// 8. Finally, the Regular Protester will then attempt to take one step in its currently facing direction (which, by the way, might have just been changed by one of the last few steps above).
	// 9. If the Regular Protester is for some reason blocked from taking a step in its currently facing direction, it will set the numSquaresToMoveInCurrentDirection to zero, resulting in a new direction being chosen during 
	// the Regular Protester’s next non-resting tick (but not the current tick –the Regular Protester must do nothing during the current tick).
	if (getDirection() == up)
	{
		// Check if the area the protestor is trying to move is out of bounds or has
		// Earth or a Boulder in it 
		if (getY() + 1 <= 60 && getWorld()->isEmpty(getX(), getY() + 1) == true)
			moveTo(getX(), getY() + 1);
		else
		// If the move cannot be made, set numSquaresToMoveInCurrentDirection to zero
		// repeat for all directions
			setNumSquaresToMoveInCurrentDirection(0);
	}
	else if (getDirection() == down)
	{
		if (getY() - 1 >= 0 && getWorld()->isEmpty(getX(), getY() - 1) == true)
			moveTo(getX(), getY() - 1);
		else 
			setNumSquaresToMoveInCurrentDirection(0);
	}
	else if (getDirection() == left)
	{
		if (getX() - 1 >= 0 && getWorld()->isEmpty(getX() - 1, getY()) == true)
			moveTo(getX() - 1, getY());
		else 
			setNumSquaresToMoveInCurrentDirection(0);
	}
	else if (getDirection() == right)
	{
		if (getX() + 1 <= 60 && getWorld()->isEmpty(getX() + 1, getY()) == true)
			moveTo(getX() + 1, getY());
		else
			setNumSquaresToMoveInCurrentDirection(0);
	}

	return;
}

void RegularProtestor::bribed()
{
	setLeaveTheOilFieldState(true);
	getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
}

// HARDCORE PROTESTOR IMPLEMENTATIONS // 

// When it is first created:
// 1. Each Hardcore Protester must have an image ID of TID_HARD_CORE_PROTESTER. 
// 2. Each Hardcore Protester must start out facing left. 
// 3. Each Hardcore Protester must decide how many squares, numSquaresToMoveInCurrentDirection, it will decide to move left before possibly switching its direction.
// This value must be: 8 <= numSquaresToMoveInCurrentDirection <= 60
// 4. Each Hardcore Protester starts out with 20 hit-points.
// 5. Each Hardcore Protester starts out NOT in a leave-the-oil-field state.
// 6. Each Hardcore Protester have the following graphic parameters: 
// a. It has an image depth of 0 –they’re always in the foreground
// b. It has a size of 1.0
// pg. 44

// DONE: 1, 2, 3, 4, 5, 6
// TODO: 
HardcoreProtestor::HardcoreProtestor(int x, int y, StudentWorld* studentWorld)
	: Protestor(x, y, studentWorld, TID_HARD_CORE_PROTESTER) 
{
	setHitPoints(20);
	setProtestor(2);
}

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

// DONE: 1, 2, 3a, 4a, 4b, 4c, 4d, 6a, 6b, 6c, 7a, 7b, 7c, 7d, 7e, 8a, 8b, 9, 10
// TODO: 3b, 3c, 5a, 5b, 5c, 
void HardcoreProtestor::doSomething()
{
		// Check if the protestor is alive and, if not, return
		if (getLifeStatus() == false)
			return;

		if (getHitPoints() <= 0)
			setLeaveTheOilFieldState(true);

		int direction = 0;
		if (getDirection() == up)
			direction = 1;
		if (getDirection() == down)
			direction = 2;
		if (getDirection() == left)
			direction = 3;
		if (getDirection() == right)
			direction = 4;

		if (getTicks() < getTicksToWaitBetweenMoves())
		{
			// Protestor is in a rest state so update ticks count and return
			setTicks(getTicks() + 1);
			return;
		}

		// 3. Otherwise, if the Hardcore Protester is in a leave-the-oil-field state (because their hit points reached zero due to being repeatedly squirted by the TunnelMan’s squirt gun or bonked by a falling Boulder), then: 
			// a. If the Hardcore Protester is at its exit point (at location x = 60, y = 60) then it will immediately set its status to dead so that it will be removed from the game at the end of the current tick (by your StudentWorld class).
			// b. Otherwise, the Hardcore Protester must move one square closer to its exit point (at x = 60, y = 60). It must determine what direction to move based on a queue-based maze-searching algorithm like the one we learned in class.
			// The Hardcore Protester may not walk through Earth or Boulders to reach its exit point (meaning that none of the 16 squares that comprise the 4x4 Hardcore Protester icon may overlap with an Earth object and that the Hardcore 
			// Protester must not come within (within means <= ) a distance of 3.0 units of any Boulders).
			// Hint: It’s inefficient for every single Hardcore Protestor to try to figure out its own optimal path to the exit, so see if you can figure out a way to compute a single data structure that enables all Protesters to 
			// quickly determine their optimal path to their common exitpoint of 60, 60, no matter where they are located in the oil field.
			// c. After moving one step toward their exit point, the Hardcore Protester will return immediately.
			// It will not perform any of the following steps.

		if (getLeaveTheOilFieldState() == true)
		{

			if (getX() == 60 && getY() == 60)
			{
				kill();
				return;
			}
			else
			{
				// 1 = LEFT
				// 2 = RIGHT
				// 3 = DOWN
				// 4 = UP
				/*int minDir = 2;
				if (protestorQueueMazeSearch (getX(), getY(), 60, 60, minDir) == true)
				{
					if (minDir == 1 && getX() - 1 >= 0)
					{
						setDirection(left);
						moveTo(getX() - 1, getY());
					}
					else if (minDir == 2 && getX() + 1 <= 60)
					{
						setDirection(right);
						moveTo(getX() + 1, getY());
					}
					else if (minDir == 3 && getY() - 1 >= 0)
					{
						setDirection(down);
						moveTo(getX(), getY() - 1);
					}
					else if (minDir == 4 && getY() + 1 <= 60)
					{
						setDirection(up);
						moveTo(getX(), getY() + 1);
					}
				}*/
				return;
			}

			setTicks(0);
			setNonRestingTicks(getNonRestingTicks() + 1);
			setNonRestingPerpindicularTicks(getNonRestingPerpindicularTicks() + 1);
			return;
		}
		// 4. Otherwise, the Hardcore Protester will check to see if they are within a distance of 4 units of the TunnelMan, AND they are currently facing in the TunnelMan’s direction. 
		// If both are true and the Hardcore Protester hasn’t shouted within its last non-resting 15 ticks, then the Hardcore Protester will: 
			// a. Play the shouting sound: SOUND_PROTESTER_YELL
			// b. Inform the TunnelMan that he’s been annoyed for a total of 2 annoyance points (deducting 2 points from the TunnelMan’s hit points, and possibly causing the TunnelMan to become sufficiently annoyed to quit the level (aka die)).
			// c. Update some state variable in a manner that prevents this Hardcore Protester from shouting again for at least 15 non-resting ticks.
			// d. Return immediately.
		else if (getWorld()->distanceToTunnelMan(getX(), getY()) <= 4.00) // Check if the protestor is facing TunnelMan's direction
		{
			// Reset the ticks counter
			// Increment the non-resting ticks counter
			setTicks(0);
			setNonRestingTicks(getNonRestingTicks() + 1);
			setNonRestingPerpindicularTicks(getNonRestingPerpindicularTicks() + 1);


			// Each if statement checks if the protestor is facing the same direction as the tunnelman
			if (getDirection() == up)
			{
				if (getX() < getWorld()->getTunnelMan()->getX() + 4 && getX() > getWorld()->getTunnelMan()->getX() - 4
					&& getY() >= getWorld()->getTunnelMan()->getY() - 4 && getY() <= getWorld()->getTunnelMan()->getY())
				{
					// Check if 15 non-resting ticks have passed and, if so, shout
					if (getNonRestingTicks() >= 15)
					{
						// Reset the non-resting ticks counter 
						setNonRestingTicks(0);
						getWorld()->playSound(SOUND_PROTESTER_YELL);
						// ANNOY TUNNELMAN
						getWorld()->getTunnelMan()->annoyed(2);
					}
				}
			}

			else if (getDirection() == down)
			{
				if (getX() < getWorld()->getTunnelMan()->getX() + 4 && getX() > getWorld()->getTunnelMan()->getX() - 4
					&& getY() >= getWorld()->getTunnelMan()->getY() && getY() <= getWorld()->getTunnelMan()->getY() + 4)
				{
					// Check if 15 non-resting ticks have passed and, if so, shout
					if (getNonRestingTicks() >= 15)
					{
						// Reset the non-resting ticks counter 
						setNonRestingTicks(0);
						getWorld()->playSound(SOUND_PROTESTER_YELL);
						// ANNOY TUNNELMAN
						getWorld()->getTunnelMan()->annoyed(2);
					}
				}
			}

			else if (getDirection() == right)
			{
				if (getX() <= getWorld()->getTunnelMan()->getX() && getX() >= getWorld()->getTunnelMan()->getX() - 4
					&& getY() > getWorld()->getTunnelMan()->getY() - 4 && getY() < getWorld()->getTunnelMan()->getY() + 4)
				{
					// Check if 15 non-resting ticks have passed and, if so, shout
					if (getNonRestingTicks() >= 15)
					{
						// Reset the non-resting ticks counter 
						setNonRestingTicks(0);
						getWorld()->playSound(SOUND_PROTESTER_YELL);
						// ANNOY TUNNELMAN
						getWorld()->getTunnelMan()->annoyed(2);
					}
				}
			}

			else if (getDirection() == left)
			{
				if (getX() <= getWorld()->getTunnelMan()->getX() + 4 && getX() >= getWorld()->getTunnelMan()->getX()
					&& getY() > getWorld()->getTunnelMan()->getY() - 4 && getY() < getWorld()->getTunnelMan()->getY() + 4)
				{
					// Check if 15 non-resting ticks have passed and, if so, shout
					if (getNonRestingTicks() >= 15)
					{
						// Reset the non-resting ticks counter 
						setNonRestingTicks(0);
						getWorld()->playSound(SOUND_PROTESTER_YELL);
						// ANNOY TUNNELMAN
						getWorld()->getTunnelMan()->annoyed(2);
					}
				}
			}

			return;
		}
		
		// 5. Otherwise, if the Hardcore Protester is more than 4.0 units away from the TunnelMan, the Hardcore Protester will:
			// a. Compute a value M, such that M = 16 + current_level_number * 2
			// b. If the Hardcore Protester is less than or equal to a total of M legal horizontal or vertical moves away from the current location of the TunnelMan (as can be determined using the same 
			// basic queue-based maze-searching algorithm described in Hardcore Protester item 3.b above), then the Hardcore Protester will sense the TunnelMan’s cell phone signal and know where he is. 
			// This is true even if the Hardcore Protester has no direct line of sight to the TunnelMan, so long as he can be reached in M moves without digging through Earth or going through a Boulder. 
			// In such a situation, the Hardcore Protester will:
				// i. Determine which horizontal/vertical direction to move in (as dictated by its maze-searching algorithm) such that if it were to make such a move it would be one square closer to the TunnelMan.
				// ii. Change its current direction to face this new direction.
				// iii. Move one square in this direction such that after the move it is one square closer to the TunnelMan’s current location.
				// iv. Return immediately
		else if (getWorld()->distanceToTunnelMan(getX(), getY()) > 4.00)
		{

			int M = 16 + getWorld()->getLevel() * 2;
			// Reset the ticks counter
			// Increment the non-resting ticks counter
			setTicks(0);
			setNonRestingTicks(getNonRestingTicks() + 1);
			setNonRestingPerpindicularTicks(getNonRestingPerpindicularTicks() + 1);

			int finalDirection = 0;
			if (protestorQueueMazeSearch(getX(), getY(), getWorld()->getTunnelMan()->getX(), getWorld()->getTunnelMan()->getY()) == 1)
			{
				if (getWorld()->isEmpty(getX() - 1, getY()) == true)
				{
					setDirection(left);
					moveTo(getX() - 1, getY());
					return;
				}
			}
			else if (protestorQueueMazeSearch(getX(), getY(), getWorld()->getTunnelMan()->getX(), getWorld()->getTunnelMan()->getY()) == 2)
			{
				if (getWorld()->isEmpty(getX() + 1, getY()) == true)
				{
					setDirection(right);
					moveTo(getX() + 1, getY());
					return;
				}
			}

			else if (protestorQueueMazeSearch(getX(), getY(), getWorld()->getTunnelMan()->getX(), getWorld()->getTunnelMan()->getY()) == 3)
			{
				if (getWorld()->isEmpty(getX(), getY() - 1) == true)
				{
					setDirection(down);
					moveTo(getX(), getY() - 1);
					return;
				}
			}

			else if (protestorQueueMazeSearch(getX(), getY(), getWorld()->getTunnelMan()->getX(), getWorld()->getTunnelMan()->getY()) == 4)
			{
				if (getWorld()->isEmpty(getX(), getY() + 1) == true)
				{
					setDirection(up);
					moveTo(getX(), getY() + 1);
					return;
				}
			}

			// 6. Otherwise, if the Hardcore Protester:
			// a. Is in a straight horizontal or vertical line of sight to the TunnelMan (even if the Hardcore Protester isn’t currently facing the TunnelMan), and 
			// b. Is more than 4 units away from the TunnelMan –that is, the radius from the Hardcore Protester and the TunnelMan is greater than 4.0 units away, and
			// c. Could actually move the entire way to the TunnelMan with no Earth or Boulders blocking its path (assuming it kept walking straight over the next N turns) 
			// then the Hardcore Protester will:
				// a. Change its direction to face in the direction of the TunnelMan, AND then take one step toward him.
				// b. The Hardcore Protester will set its numSquaresToMoveInCurrentDirection value to zero, forcing it to pick a new direction/distance to move during 
				// its next non-resting tick (unless of course, the Hardcore Protester still sees the TunnelManin its line of sight, in which case it will continue to move toward the TunnelMan).
				// c. Then the Hardcore Protester will immediately return.
			if ((getWorld()->getTunnelMan()->getX() == getX() || getWorld()->getTunnelMan()->getY() == getY()))
			{
				// Make sure the path to the tunnelman is clear
				if (getWorld()->isPathClear(getX(), getY(), 1) == true)
				{
					// Set direction towards the tunnelman
					setDirection(up);

					// Move up if possible
					if (getY() + 1 <= 60 && getWorld()->isEmpty(getX(), getY() + 1) == true)
						moveTo(getX(), getY() + 1);
				}
				else if (getWorld()->isPathClear(getX(), getY(), 2) == true)
				{
					setDirection(down);
					if (getY() - 1 >= 0 && getWorld()->isEmpty(getX(), getY() - 1) == true)
						moveTo(getX(), getY() - 1);
				}
				else if (getWorld()->isPathClear(getX(), getY(), 3) == true)
				{
					setDirection(left);
					if (getX() - 1 >= 0 && getWorld()->isEmpty(getX() - 1, getY()) == true)
						moveTo(getX() - 1, getY());
				}
				else if (getWorld()->isPathClear(getX(), getY(), 4) == true)
				{
					setDirection(right);
					if (getX() + 1 <= 60 && getWorld()->isEmpty(getX() + 1, getY()) == true)
						moveTo(getX() + 1, getY());
				}
			}

			setNumSquaresToMoveInCurrentDirection(0);
			return;
		}
		// 7. Otherwise, the Hardcore Protester can’t directly see the TunnelMan. As such, it will decrement its numSquaresToMoveInCurrentDirection variable by one.
		else
		{
			// Reset the ticks counter
			// Increment the non-resting ticks counter
			setTicks(0);
			setNonRestingTicks(getNonRestingTicks() + 1);
			setNonRestingPerpindicularTicks(getNonRestingPerpindicularTicks() + 1);
			setNumSquaresToMoveInCurrentDirection(getNumSquaresToMoveInCurrentDirection() - 1);
		}
		 // If the Hardcore Protester has finished walking numSquaresToMoveInCurrentDirection steps in its currently-selected direction (i.e.,numSquaresToMoveInCurrentDirection <= 0), then:
			// a. The Hardcore  Protester will  pick  a  random  new  direction  to  move  (up, down, left or right).
			// b. If the random direction is blocked either by Earth or a Boulder such that it can’t take even a single step in that chosen direction, then it will select a different direction and check it for
			// blockage (it will continue checking directions until it has picked a direction that is not blocked).
			// c. The Hardcore Protester will then change its direction to this new chosen direction.
			// d. The Hardcore Protester will then pick a new value for numSquaresToMoveInCurrentDirection that will govern how far it should move in the selected direction over the next numSquaresToMoveInCurrentDirection non-resting ticks.
			// The value must be such that: 8 <= numSquaresToMoveInCurrentDirection<= 60.
		if (getNumSquaresToMoveInCurrentDirection() <= 0)
		{
			for (;;)
			{
				int randomDirection = rand() % 4 + 1;
				if (randomDirection == 1)
				{
					if (getWorld()->isEmpty(getX(), getY() + 1) == true)
					{
						setDirection(up);
						break;
					}
				}
				if (randomDirection == 2)
				{
					if (getWorld()->isEmpty(getX(), getY() - 1) == true)
					{
						setDirection(down);
						break;
					}
				}
				if (randomDirection == 3)
				{
					if (getWorld()->isEmpty(getX() - 1, getY()) == true)
					{
						setDirection(left);
						break;
					}
				}
				if (randomDirection == 4)
				{
					if (getWorld()->isEmpty(getX() + 1, getY()) == true)
					{
						setDirection(right);
						break;
					}
				}
			}
			setNumSquaresToMoveInCurrentDirection();
		}
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
		else if (getNonRestingPerpindicularTicks() >= 200)
		{
			setNonRestingPerpindicularTicks(0);
			if (getDirection() == left || getDirection() == right)
			{
				if (getWorld()->isEmpty(getX(), getY() + 1) == true && getWorld()->isEmpty(getX(), getY() - 1) == false)
					setDirection(up);
				else if (getWorld()->isEmpty(getX(), getY() + 1) == false && getWorld()->isEmpty(getX(), getY() - 1) == true)
					setDirection(down);
				else if (getWorld()->isEmpty(getX(), getY() + 1) == true && getWorld()->isEmpty(getX(), getY() - 1) == true)
				{
					int randomWay = rand() % 2 + 1;
					if (randomWay == 1)
						setDirection(up);
					else
						setDirection(down);
				}
				if (getDirection() == up || getDirection() == down)
				{
					setNumSquaresToMoveInCurrentDirection();
					setNonRestingPerpindicularTicks(0);
				}
			}
			else if (getDirection() == up || getDirection() == down)
			{
				if (getWorld()->isEmpty(getX() + 1, getY()) == true && getWorld()->isEmpty(getX() - 1, getY()) == false)
					setDirection(right);
				else if (getWorld()->isEmpty(getX() + 1, getY()) == false && getWorld()->isEmpty(getX() - 1, getY()) == true)
					setDirection(left);
				else if (getWorld()->isEmpty(getX() + 1, getY()) == true && getWorld()->isEmpty(getX() - 1, getY()) == true)
				{
					int randomWay = rand() % 2 + 1;
					if (randomWay == 1)
						setDirection(right);
					else
						setDirection(left);
				}
				if (getDirection() == left || getDirection() == right)
				{
					setNumSquaresToMoveInCurrentDirection();
					setNonRestingPerpindicularTicks(0);
				}
			}
		}

		// 9. Finally, the Hardcore Protester will then attempt to take one step in its currently facing direction (which, by the way, might have just been changed by one of the last few steps above).
		// 10. If the Hardcore Protester is for some reason blocked from taking a step in its currently facing direction, it will set the numSquaresToMoveInCurrentDirection to zero, resulting in a new direction being chosen during 
		// the Hardcore Protester’s next non-resting tick (but not the current tick –the Hardcore Protester must do nothing during the current tick).
		// Note: Only Earth and Boulders block a Hardcore Protester from moving. Hardcore Protesters can occupy the same square as all other game objects including the TunnelMan and other Protesters.
		if (getDirection() == up)
		{
			if (getY() + 1 <= 60 && getWorld()->isEmpty(getX(), getY() + 1) == true)
				moveTo(getX(), getY() + 1);
			else
				setNumSquaresToMoveInCurrentDirection(0);
		}
		else if (getDirection() == down)
		{
			if (getY() - 1 >= 0 && getWorld()->isEmpty(getX(), getY() - 1) == true)
				moveTo(getX(), getY() - 1);
			else
				setNumSquaresToMoveInCurrentDirection(0);
		}
		else if (getDirection() == left)
		{
			if (getX() - 1 >= 0 && getWorld()->isEmpty(getX() - 1, getY()) == true)
				moveTo(getX() - 1, getY());
			else
				setNumSquaresToMoveInCurrentDirection(0);
		}
		else if (getDirection() == right)
		{
			if (getX() + 1 <= 60 && getWorld()->isEmpty(getX() + 1, getY()) == true)
				moveTo(getX() + 1, getY());
			else
				setNumSquaresToMoveInCurrentDirection(0);
		}

		return;

}

// NAME: bribed
// INPUT: NONE
// OUTPUT: NONE 
// DESCRIPTION: puts the protestor into a leave-the-oil-field-state and
// plays the correct sound
void HardcoreProtestor::bribed()
{
	int ticks_to_stare = max(50, int(100 - getWorld()->getLevel() * 10));
	setTicks(getTicks() - ticks_to_stare);
	getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
}

Point::Point(int x, int y)
{
	m_x = x;
	m_y = y;
}

int Point::getx() const
{
	return m_x;
}

int Point::gety() const
{
	return m_y;
}
