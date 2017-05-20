#include "CombatManager.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

CombatManager::CombatManager():
	discoveredUnits(new std::set<const Unit>())
{
	
}

// Manage squads (send them to attack, make them patrol, defend)
void CombatManager::update()
{
	for (Squad* squad : squads)
	{
		switch (squad->getMode())
		{
			case Squad::ATTACK :
				aggressiveMode(squad);
				break;
			case Squad::PATROL :
				patrolMode(squad);
				break;
			default :
				defensiveMode(squad);
				break;
		}
	}
}

// Add a new squad to the squad set
void CombatManager::addSquad(Squad* squad)
{
	this->squads.insert(squad);
}

// Get the squad with the given id
Squad* CombatManager::findSquad(int id)
{
	Squad *squadResult = 0;
	for (Squad* squad : squads)
	{
		if (squad->getSquadId() == id)
		{
			squadResult = squad;
			break;
		}
	}

	return 
		squadResult;
}

// Get the number of squads
int CombatManager::numberOfSquad()
{
	return 
		this->squads.size();
}

// Get the set of squads
set<Squad*> CombatManager::getSquadList()
{
	return 
		this->squads;
}

// Make the given squad defend its position
void CombatManager::defensiveMode(Squad* squad)
{
	//TODO
}

// Make the given squad attack its target 
void CombatManager::aggressiveMode(Squad* squad)
{
	std::set<Unit> unitToErase;
	attackingLogic(&unitToErase, squad->getGroundUnits(), squad, true);
	attackingLogic(&unitToErase, squad->getAerialUnits(), squad, false);

	for (Unit unitErase : unitToErase)
	{
		if (unitErase->isFlying())
		{
			squad->getAerialUnits()->erase(unitErase);
		}
		else
		{
			squad->getGroundUnits()->erase(unitErase);
		}
	}
}

// Make the given squad patrol on its given path
void CombatManager::patrolMode(Squad* squad)
{
	//TODO
}

// Store the set of discovered units
void CombatManager::setDiscoveredUnits(set<const Unit> *units)
{
	this->discoveredUnits = units;
}

// Store the set of all the base locations
void CombatManager::setAllBaseLocations(set<Base*> newAllBaseLocations)
{
	for (Base* b : newAllBaseLocations)
	{
		allBaseLocations.insert(b);
	}
}

// Make the given squad attack its target (used by aggressiveMode)
void CombatManager::attackingLogic(set<Unit> *erase, set<const Unit> *unitType, Squad *squad, bool isGrounded)
{
	int sizeUnit = unitType->size();
	Unit unitClose = 0;
	float distanceClose = 0;
	
	for (Unit unit : *unitType)
	{
		if (unit->exists())
		{
			if (unitClose == 0)
			{
				bool attack = false;
				Unit temp = 0;
				if (unitClose != 0)
					attack = true;

				bool attacker = false;
				int distanceAttacker = 0;

				for (Unit unitEnemy : *discoveredUnits)
				{
					if (unitEnemy->getType() != UnitTypes::Resource_Vespene_Geyser &&
						unitEnemy->getType() != UnitTypes::Zerg_Larva && 
						unitEnemy->getType() != UnitTypes::Zerg_Egg &&
						((isGrounded && !unitEnemy->isFlying()) || !isGrounded))
					{
						float tempDist = (float)unitEnemy->getPosition().getDistance(unit->getPosition());
						if (distanceClose == 0)
							distanceClose = tempDist;

						if (unitEnemy->isAttacking())
						{
							if ((distanceAttacker == 0) || (tempDist < 600 && distanceAttacker >= tempDist))
							{
								distanceAttacker = (int)tempDist;
								unitClose = unitEnemy;
								attacker = true;
							}
						}

						else if (tempDist < 600 && distanceClose >= tempDist && unitEnemy->canAttack() && unitEnemy->getType() != (BWAPI::UnitTypes::Zerg_Larva || BWAPI::UnitTypes::Resource_Vespene_Geyser) && !attacker)
						{
							distanceClose = tempDist;
							unitClose = unitEnemy;
							attack = true;
						}
						else if (tempDist < 600 && !attack && !attacker && distanceClose >= tempDist && unitEnemy->getType() != (BWAPI::UnitTypes::Zerg_Larva || BWAPI::UnitTypes::Resource_Vespene_Geyser))
						{
							temp = unitEnemy;
						}
					}
				}

				if (!attack && !attacker)
					unitClose = temp;

				if (unitClose != 0)
				{
					squad->setTargetLocation(unitClose->getPosition());
					unit->attack(unitClose);
				}
				else if (squad->getTargetLocation().x != 0 && squad->getTargetLocation().y != 0 && !ToolBox::IsInCircle(squad->getTargetLocation(), 100, unit->getPosition(), 10))
				{
					unit->attack(squad->getTargetLocation());
				}
				else if (squad->getTargetLocation().x != 0 && squad->getTargetLocation().y != 0)
				{
					Base* baseToFocusNext = nullptr;
					Position currentTarget = squad->getTargetLocation();
					float tmpDist = INFINITY;

					for (Base* b : allBaseLocations)
					{
						if (!b->isInvalidToGroundUnits && b->hasToBeChecked() && currentTarget.getDistance(b->getPosition()) < tmpDist)
						{
							tmpDist = (float)currentTarget.getDistance(b->getPosition());
							baseToFocusNext = b;
						}
					}

					if (baseToFocusNext != nullptr)
					{
						squad->setTargetLocation(baseToFocusNext->getPosition());
					}
				}
			}
			else if (!unitClose->exists() && (float)unitClose->getPosition().getDistance(unit->getPosition()) > 600)
			{
				unitClose = 0;
			}	
			else
			{
				if (unitClose != 0)
				{
					squad->setTargetLocation(unitClose->getPosition());
					unit->attack(unitClose);
				}
				else if (squad->getTargetLocation().x != 0 && squad->getTargetLocation().y != 0)
				{
					unit->attack(squad->getTargetLocation());
				}
			}
		}
		else erase->insert(unit);
	}
}

// Draw the debug info of the combat manager
void CombatManager::drawDebug()
{
	Position targetLocation;

	for (Squad* squad : squads)
	{
		if (squad->getMode() == Squad::ATTACK)
		{
			targetLocation = squad->getTargetLocation();

			for (Unit grounded : *squad->getGroundUnits())
			{
				Broodwar->drawCircleMap(targetLocation, 15, Colors::Teal, true);
				Broodwar->drawLineMap(grounded->getPosition(), targetLocation, Colors::Teal);
			}

			for (Unit flying : *squad->getAerialUnits())
			{
				Broodwar->drawCircleMap(targetLocation, 15, Colors::Teal, true);
				Broodwar->drawLineMap(flying->getPosition(), targetLocation, Colors::Teal);
			}
		}
	}
}