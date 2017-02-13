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
Unit ProductionManager::getResourceDepot(unsigned int index)
{
	if (index < resourceDepots.size())
	{
		return *(next(resourceDepots.begin(), index));
	}
	else
	{
		return NULL;
	}
}

// Tell if we are about to saturate our supply
bool ProductionManager::isSupplyAboutToBeFull()
{
	return Broodwar->self()->supplyUsed() == (Broodwar->self()->supplyTotal() - (SUPPLY_FULL_CHECK_THRESHOLD * UnitTypes::Zerg_Drone.supplyRequired()));
}

// Tell if we are using all of our supplies or not
bool ProductionManager::isSupplyFull()
{
	return Broodwar->self()->supplyUsed() >= Broodwar->self()->supplyTotal();
}

// Tell if a unit is being built or not
bool ProductionManager::isUnitBeingCreated()
{
	return Broodwar->self()->incompleteUnitCount() > 0;
}
