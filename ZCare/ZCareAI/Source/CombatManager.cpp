#include "CombatManager.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

CombatManager::CombatManager()
{
	
}

void CombatManager::update()
{
	for (Squad* squad : squadList)
	{
		bool isUnderAttack = false;
		Unit unitToAttack = 0;
		std::set<Unit> unitToErase;

		for (Unit terrainUnit : squad->getTerrainUnit())
		{
			if (terrainUnit->exists())
			{
				if (terrainUnit->isUnderAttack())
				{
					isUnderAttack = true;
					unitToAttack = terrainUnit->getOrderTarget();
				}
			}
			else unitToErase.insert(terrainUnit);	
		}

		for (Unit aerialUnit : squad->getAerialUnit())
		{
			if (aerialUnit->exists())
			{
				if (aerialUnit->isUnderAttack())
				{
					isUnderAttack = true;
					unitToAttack = aerialUnit->getOrderTarget();
				}
			}
			else unitToErase.insert(aerialUnit);
		}
		for (Unit unitErase : unitToErase)
		{
			if (unitErase->isFlying())
			{
				squad->getAerialUnit().erase(unitErase);
			}
			else
			{
				squad->getTerrainUnit().erase(unitErase);
			}
		}
		squad->attackOrMove(unitToAttack);
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