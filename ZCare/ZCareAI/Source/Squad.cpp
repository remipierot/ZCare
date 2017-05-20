#include "Squad.h"

using namespace BWAPI;
using namespace std;

Squad::Squad(int squad):
	id(squad),
	capacity(20),
	currentSize(0),
	mode(ATTACK),
	targetLocation(0, 0)
{
	
}

Squad::Squad(set<const Unit> groundUnits, set<const Unit> aerialUnits)
{
	this->aerialUnits = aerialUnits;
	this->groundUnits = groundUnits;
	currentSize = getSquadSize();
	mode = ATTACK;
}

Squad::~Squad()
{

}

// Make a non aggressive move to the position or unit given
void Squad::neutralMove(PositionOrUnit target)
{
	Position lPosition;
	if (target.isUnit())
		lPosition = ToolBox::ConvertTilePosition(target.getUnit()->getTilePosition(), target.getUnit()->getBuildType());
	else lPosition = target.getPosition();

	for (Unit unitT : groundUnits)
	{
		unitT->move(lPosition);
	}
	for (Unit unitA : aerialUnits)
	{
		unitA->move(lPosition);
	}
}

// Make an aggressive move (will attack on sight) to the position or unit given
void Squad::aggressiveMove(PositionOrUnit target)
{
	for (Unit unitT : groundUnits)
	{
		if (!unitT->isAttacking())
			unitT->attack(target);
	}
	for (Unit unitA : aerialUnits)
	{
		if (!unitA->isAttacking())
			unitA->attack(target);
	}
}

// Insert a unit into the squad
bool Squad::insertUnit(Unit unit)
{
	bool unitInserted = false;

	if (this->capacity != currentSize)
	{
		if (unit->isFlying())
			aerialUnits.insert(unit);
		else groundUnits.insert(unit);
		currentSize += 1;
		unitInserted = true;
	}

	return 
		unitInserted;
}

// Get the size of the squad
int Squad::getSquadSize()
{
	return 
		groundUnits.size() + aerialUnits.size();
}

// Get the squad id
int Squad::getSquadId()
{
	return 
		this->id;
}

// Get the squad maximum capacity
int Squad::getSquadCapacity()
{
	return 
		capacity;
}

// Get the set of ground units
set<const Unit>* Squad::getGroundUnits()
{
	return 
		&this->groundUnits;
}

// Get the set of flying units
set<const Unit>* Squad::getAerialUnits()
{
	return 
		&this->aerialUnits;
}

// Set the mode of the squad
void Squad::setMode(SquadMode mode)
{
	this->mode = mode;
}

// Set the target location
void Squad::setTargetLocation(Position position)
{
	this->targetLocation = position;
}

// Get the mode
Squad::SquadMode Squad::getMode()
{
	return 
		this->mode;
}

// Get the target location
Position Squad::getTargetLocation()
{
	return 
		this->targetLocation;
}
