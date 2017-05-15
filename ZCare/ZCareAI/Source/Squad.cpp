#include "Squad.h"


Squad::Squad(int squad):
	idSquad(squad),
	numberUnitMax(20),
	currentNumberUnit(0),
	modeSquad(defenseMode),
	positionObjective(0,0)
{
	
}

Squad::Squad(std::set<const BWAPI::Unit> terrainUnit, std::set<const BWAPI::Unit> aerialUnit)
{
	this->aerialUnit = aerialUnit;
	this->terrainUnit = terrainUnit;
	currentNumberUnit = numberUnit();
	modeSquad = defenseMode;
}

Squad::~Squad()
{

}

void Squad::moveToLocation(BWAPI::PositionOrUnit target)
{
	BWAPI::Position lPosition;
	if (target.isUnit())
		lPosition = ToolBox::ConvertTilePosition(target.getUnit()->getTilePosition(), target.getUnit()->getBuildType());
	else lPosition = target.getPosition();

	for (BWAPI::Unit unitT : terrainUnit)
	{
		unitT->move(lPosition);
	}
	for (BWAPI::Unit unitA : aerialUnit)
	{
		unitA->move(lPosition);
	}
}


void Squad::attackOrMove(BWAPI::PositionOrUnit target)
{
	for (BWAPI::Unit unitT : terrainUnit)
	{
		if (!unitT->isAttacking())
			unitT->attack(target);
	}
	for (BWAPI::Unit unitA : aerialUnit)
	{
		if (!unitA->isAttacking())
			unitA->attack(target);
	}
}


bool Squad::insertUnit(BWAPI::Unit unit)
{
	if (this->numberUnitMax != currentNumberUnit)
	{
		if (unit->isFlying())
			aerialUnit.insert(unit);
		else terrainUnit.insert(unit);
		currentNumberUnit += 1;
		return true;
	}
	else return false;

}

int Squad::numberUnit()
{
	return terrainUnit.size() + aerialUnit.size();
}

int Squad::getNumberUnitMax()
{
	return numberUnitMax;
}

int Squad::getIdSquad()
{
	return this->idSquad;
}

std::set<const BWAPI::Unit>* Squad::getTerrainUnit()
{
	return &this->terrainUnit;
}

std::set<const BWAPI::Unit>* Squad::getAerialUnit()
{
	return &this->aerialUnit;
}

void Squad::setModeSquad(SquadMode mode)
{
	this->modeSquad = mode;
}

void Squad::setPositionObjective(BWAPI::Position position)
{
	this->positionObjective = position;
}

BWAPI::Position Squad::getPositionObjective()
{
	return this->positionObjective;
}