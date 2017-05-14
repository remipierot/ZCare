#include "CombatManager.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

CombatManager::CombatManager():
	finalAttackMode(false),
	defenseMode(true),
	attackMode(false)
{
	
}

void CombatManager::update()
{
	bool isUnderAttack = false;
	std::set<Unit> unitToAttack;
	for (Unit unit : Broodwar->self()->getUnits())
	{
		if (unit->isUnderAttack())
		{
			isUnderAttack = true;
			Unit target = unit->getOrderTarget();
			if (target!=0)
				unitToAttack.insert(unit->getOrderTarget());
		}	
	}

	int ennemyUnitCount = unitToAttack.size();
	if (ennemyUnitCount > 0)
	{
		for (Squad* squad : squadList)
		{
			if (squad->defenseMode)
			{
				std::set<Unit> unitToErase;
				for (Unit terrainUnit : *squad->getTerrainUnit())
				{
					if (terrainUnit->exists())
					{
<<<<<<< HEAD
						//Faire le traitement
						float distanceClose = 0;
						Unit unitClose = 0;
						for (Unit ennemy : unitToAttack)
=======
						float tempDist = (float)ennemy->getPosition().getDistance(terrainUnit->getPosition());
						if (unitClose == 0)
>>>>>>> origin/release
						{
							float tempDist = ennemy->getPosition().getDistance(terrainUnit->getPosition());
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
							Color color(0, 0, 25);
							Broodwar->drawCircleMap(unitClose->getPosition(), 30, color, true);
							Broodwar->drawLineMap(unitClose->getPosition(), terrainUnit->getPosition(), color);
						}

					}
					else unitToErase.insert(terrainUnit);
				}

				for (Unit aerialUnit : *squad->getAerialUnit())
				{
					if (aerialUnit->exists())
					{
<<<<<<< HEAD
						float distanceClose = 0;
						Unit unitClose = 0;
						for (Unit ennemy : unitToAttack)
=======
						float tempDist = (float)ennemy->getPosition().getDistance(aerialUnit->getPosition());
						if (unitClose == 0)
						{
							distanceClose = tempDist;
							unitClose = ennemy;
						}
						else
>>>>>>> origin/release
						{
							float tempDist = ennemy->getPosition().getDistance(aerialUnit->getPosition());
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
							aerialUnit->attack(unitClose);
							Color color(0, 25, 0);
							Broodwar->drawCircleMap(unitClose->getPosition(), 30, color, true);
							Broodwar->drawLineMap(unitClose->getPosition(), aerialUnit->getPosition(), color);

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
			else if (squad->attackMode)
			{

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