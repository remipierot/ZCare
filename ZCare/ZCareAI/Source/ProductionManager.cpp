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

// Construct a building at the specified location
bool ProductionManager::makeBuilding(const UnitType& buildingType, const TilePosition& targetBuildLocation, Unit worker)
{
	bool success = false;

	if (worker != NULL && targetBuildLocation.isValid())
	{
		success = worker->build(buildingType, targetBuildLocation);
	}

	return success;
}

// Build the wanted unit
bool ProductionManager::makeUnit(int resourceDepotIndex, const BWAPI::UnitType& unitType)
{
	Unit resourceDepot = getResourceDepot(resourceDepotIndex);
	bool success = false;

	if (resourceDepot->getType().isResourceDepot() && resourceDepot->isIdle())
	{
		success = resourceDepot->train(unitType);
	}

	return success;
}

// Tell if we are about to saturate our supply
bool ProductionManager::isSupplyAboutToBeFull()
{
	return Broodwar->self()->supplyUsed() >= (Broodwar->self()->supplyTotal() - (SUPPLY_FULL_CHECK_THRESHOLD * UnitTypes::Zerg_Drone.supplyRequired()));
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

// Return the closest unit of the given type using the base at the given index
Unit ProductionManager::getClosestUnit(int resourceDepotIndex, BWAPI::UnitType unitType)
{
	return getResourceDepot(resourceDepotIndex)->getClosestUnit(GetType == unitType && Exists, 256);
}

// Return the quantity of mineral possessed by the player
int ProductionManager::getMineralCount()
{
	return Broodwar->self()->minerals();
}

// Return the quantity of vespene gas possessed by the player
int ProductionManager::getVespeneCount()
{
	return Broodwar->self()->gas();
}

// Return the closest buildable position for a given building using the position given
TilePosition ProductionManager::getClosestBuildablePosition(UnitType buildingType, TilePosition wantedPosition)
{
	return Broodwar->getBuildLocation(buildingType, wantedPosition);
}
