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
		for (Unit terrainUnit : squad->getTerrainUnit)
		{
			if (terrainUnit->isUnderAttack)
			{
				isUnderAttack = true;
				unitToAttack = terrainUnit->getOrderTarget();
			}
		}

		for (Unit aerialUnit : squad->getTerrainUnit)
		{
			if (aerialUnit->isUnderAttack)
			{
				isUnderAttack = true;
				unitToAttack = aerialUnit->getOrderTarget();
			}
		}
		squad->attackOrMove(unitToAttack);
	}
}

void CombatManager::addSquad(Squad* squad)
{
	squadList.insert(squad);
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