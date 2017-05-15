#include "CombatManager.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

CombatManager::CombatManager():
	finalAttackMode(false),
	defenseMode(true),
	attackMode(false),
	unitToAttack(new std::set<Unit>()),
	unitDiscover(new std::set<const Unit>())
{
	
}

void CombatManager::update()
{
	bool isUnderAttack = false;
	///std::set<Unit> unitToAttack;
	this->unitToAttack->clear();
	for (Unit unit : Broodwar->self()->getUnits())
	{
		if (unit->isUnderAttack())
		{
			isUnderAttack = true;
			Unit target = unit->getOrderTarget();
			if (target!=0)
				this->unitToAttack->insert(unit->getOrderTarget());
		}	
	}
	
	int ennemyUnitCount = this->unitToAttack->size();
	if (ennemyUnitCount > 0)
	{
		for (Squad* squad : squadList)
		{
			if (squad->defenseMode)
			{
				modeDefense(squad);
			}
			else if (squad->attackMode)
			{
				modeAttack(squad);
			}
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
			//Faire le traitement
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
	traitementAttack(&unitToErase, squad->getTerrainUnit(),squad);
	traitementAttack(&unitToErase, squad->getAerialUnit(),squad);

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

void CombatManager::setUnitDiscover(std::set<const Unit> *unit)
{
	this->unitDiscover = unit;
}

void CombatManager::setBase(std::set<Base*> *base)
{
	this->baseStruct = base;
}

void CombatManager::traitementAttack(std::set<Unit> *erase, std::set<const Unit> *unitType, Squad *squad)
{
	int sizeUnit = unitType->size();
	for (Unit unit : *unitType)
	{
		if (unit->exists())
		{
			if (unit->getUnitsInRadius(300).size() <= sizeUnit)
			{
				float distanceClose = 0;
				Unit unitClose = 0;
				for (Unit ennemy : *unitToAttack)
				{
					float tempDist = (float)ennemy->getPosition().getDistance(unit->getPosition());
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
				}

				for (Unit unitEnemy : *unitDiscover)
				{
					float tempDist = (float)unitEnemy->getPosition().getDistance(unit->getPosition());
					if (tempDist < 300 && distanceClose > tempDist)
					{
						if (unitEnemy->canAttack() || unitClose == 0)
						{
							distanceClose = tempDist;
							unitClose = unitEnemy;
						}
					}
				}

				if (unitClose != 0)
				{
					unit->attack(unitClose);
					Color color(0, 0, 255);
					Broodwar->drawCircleMap(unitClose->getPosition(), 15, color, true);
					Broodwar->drawLineMap(unitClose->getPosition(), unit->getPosition(), color);
				}
				else if (squad->getPositionObjective().x != 0 && squad->getPositionObjective().y !=0)
					unit->attack(squad->getPositionObjective());
			}
			else
			{
				BWAPI::Position posBase;
				for (Base *base : *baseStruct)
				{
					if (base->isStartingLocation && !base->isEnnemyLocation)
					{
						posBase = base->position;
						break;
					}
						
				}
				unit->move(posBase);//ON FUIT A LA BASE
			}

		}
		else erase->insert(unit);
	}
}

std::set<Base*> *CombatManager::getBase()
{
	return this->baseStruct;
}