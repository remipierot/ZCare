#include "ProductionManager.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

// Fill the base set
void ProductionManager::updateResourceDepots()
{
	for (auto &u : Broodwar->self()->getUnits())
	{
		if (u->getType().isResourceDepot())
		{
			resourceDepots.insert(u);
		}
	}
}

// Return the wanted base
Unit ProductionManager::getResourceDepot(int index)
{
	if (index >= 0 && index < resourceDepots.size())
	{
		return *(next(resourceDepots.begin(), index));
	}
	else
	{
		return NULL;
	}
}
