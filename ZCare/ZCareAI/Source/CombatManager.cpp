#include "CombatManager.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

CombatManager::CombatManager():
	unitToAttack(new std::set<Unit>()),
	unitDiscover(new std::set<const Unit>())
{
	
}

void CombatManager::update()
{
	bool isUnderAttack = false;
	this->unitToAttack->clear();
	for (Unit unit : Broodwar->self()->getUnits())
	{
		if (unit->isUnderAttack())
		{
			isUnderAttack = true;
			Unit target = unit->getOrderTarget();
			if (target!=0)
				this->unitToAttack->insert(target);
		}	
	}

	for (Unit unit: *unitToAttack)
	{
		Broodwar->drawCircleMap(unit->getPosition(), 20, Color(255, 0, 0));
	}

	for (Squad* squad : squadList)
	{
		if (squad->getModeSquad() == Squad::defenseMode)
		{
			modeDefense(squad);
		}
		else if (squad->getModeSquad() == Squad::attackMode)
		{
			modeAttack(squad);
		}
	}
}

void CombatManager::addSquad(Squad* squad)
{
	this->squadList.insert(squad);
}

Squad* CombatManager::findSquad(int idSquad)
{
	Squad *squadResult = 0;
	for (Squad* squad : squadList)
	{
		if (squad->getIdSquad() == idSquad)
		{
			squadResult = squad;
			break;
		}
	}
	return squadResult;
}

int CombatManager::squadNumber()
{
	return this->squadList.size();
}

std::set<Squad*> CombatManager::getSquadList()
{
	return this->squadList;
}

void CombatManager::modeDefense(Squad* squad)
{
	std::set<Unit> unitToErase;
	for (Unit terrainUnit : *squad->getTerrainUnit())
	{
		if (terrainUnit->exists())
		{
			float distanceClose = 0;
			Unit unitClose = 0;
			for (Unit ennemy : *unitToAttack)
			{
				float tempDist = (float)ennemy->getPosition().getDistance(terrainUnit->getPosition());
				if (tempDist < 300)
				{
					if (unitClose == 0)
					{
						distanceClose = tempDist;
						unitClose = ennemy;
					}
					else
					{
						if (distanceClose > tempDist)
						{
							distanceClose = tempDist;
							unitClose = ennemy;
						}
					}
				}
				
			
				if (unitClose != 0)
				{
					terrainUnit->attack(unitClose);
					Color color(0, 0, 255);
					Broodwar->drawCircleMap(unitClose->getPosition(), 15, color, true);
					Broodwar->drawLineMap(unitClose->getPosition(), terrainUnit->getPosition(), color);
				}
			}
		}
		else unitToErase.insert(terrainUnit);
	}

	for (Unit aerialUnit : *squad->getAerialUnit())
	{
		if (aerialUnit->exists())
		{
			float distanceClose = 0;
			Unit unitClose = 0;
			for (Unit ennemy : *unitToAttack)
			{
				float tempDist = (float)ennemy->getPosition().getDistance(aerialUnit->getPosition());
				if (tempDist < 600)
				{
					if (unitClose == 0)
					{
						distanceClose = tempDist;
						unitClose = ennemy;
					}
					else
					{
						if (distanceClose > tempDist)
						{
							distanceClose = tempDist;
							unitClose = ennemy;
						}
					}

					if (unitClose != 0)
					{
						aerialUnit->attack(unitClose);
						Color color(0, 25, 0);
						Broodwar->drawCircleMap(unitClose->getPosition(), 30, color, true);
						Broodwar->drawLineMap(unitClose->getPosition(), aerialUnit->getPosition(), color);
					}
				}
				
			}
		}
		else unitToErase.insert(aerialUnit);
	}

	for (Unit unitErase : unitToErase)
	{
		if (unitErase->isFlying())
		{
			squad->getAerialUnit()->erase(unitErase);
		}
		else
		{
			squad->getTerrainUnit()->erase(unitErase);
		}
	}
}

void CombatManager::modeAttack(Squad* squad)
{
	std::set<Unit> unitToErase;
	traitementAttack(&unitToErase, squad->getTerrainUnit(),squad, true);
	traitementAttack(&unitToErase, squad->getAerialUnit(),squad, false);

	for (Unit unitErase : unitToErase)
	{
		if (unitErase->isFlying())
		{
			squad->getAerialUnit()->erase(unitErase);
		}
		else
		{
			squad->getTerrainUnit()->erase(unitErase);
		}
	}
}

void CombatManager::setDiscoveredUnits(std::set<const Unit> *unit)
{
	this->unitDiscover = unit;
}

void CombatManager::setBase(std::set<Base*> *base)
{
	this->baseStruct = base;
}

void CombatManager::traitementAttack(std::set<Unit> *erase, std::set<const Unit> *unitType, Squad *squad, bool isGrounded)
{
	int sizeUnit = unitType->size();
	Unit unitClose = 0;
	float distanceClose = 0;
	
	/*
	// Units alive to have before starting an attack
	if (sizeUnit < 6)
	{
		return;
	}
	*/

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

				for (Unit unitEnemy : *unitDiscover)
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
								distanceAttacker = tempDist;
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
					unit->attack(unitClose);
					Color color(0, 0, 255);
					Broodwar->drawCircleMap(unitClose->getPosition(), 15, color, true);
					Broodwar->drawLineMap(unitClose->getPosition(), unit->getPosition(), color);
				}
				else if (squad->getPositionObjective().x != 0 && squad->getPositionObjective().y != 0 && !ToolBox::IsInCircle(squad->getPositionObjective(), 100, unit->getPosition(), 10))
				{
					unit->attack(squad->getPositionObjective());
					Broodwar->drawLineMap(squad->getPositionObjective(), unit->getPosition(), Color(255, 0, 0));
				}
				else if (squad->getPositionObjective().x != 0 && squad->getPositionObjective().y != 0)
				{
					Base* baseToFocusNext = nullptr;
					Position currentTarget = squad->getPositionObjective();
					float tmpDist = INFINITY;

					for (Base* b : *baseStruct)
					{
						if (!b->isInvalidToGroundUnits && b->hasToBeChecked() && currentTarget.getDistance(b->getPosition()) < tmpDist)
						{
							tmpDist = currentTarget.getDistance(b->getPosition());
							baseToFocusNext = b;
						}
					}

					if (baseToFocusNext != nullptr)
					{
						squad->setPositionObjective(baseToFocusNext->getPosition());
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
					unit->attack(unitClose);
					Color color(0, 0, 255);
					Broodwar->drawCircleMap(unitClose->getPosition(), 15, color, true);
					Broodwar->drawLineMap(unitClose->getPosition(), unit->getPosition(), color);
				}
				else if (squad->getPositionObjective().x != 0 && squad->getPositionObjective().y != 0)
				{
					unit->attack(squad->getPositionObjective());
					Broodwar->drawLineMap(squad->getPositionObjective(), unit->getPosition(), Color(255, 0, 0));
				}
			}
		}
		else erase->insert(unit);
	}
}

std::set<Base*> *CombatManager::getBase()
{
	return this->baseStruct;
}