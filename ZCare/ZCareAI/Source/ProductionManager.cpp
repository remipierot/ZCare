#include "ProductionManager.h"
#include <vector>

using namespace std;
using namespace BWAPI;
using namespace Filter;

// Fill the base set
void ProductionManager::updateResourceDepots()
{
	for (auto &u : Broodwar->self()->getUnits())
	{
		if (u->getType().isResourceDepot() && (find(resourceDepots.begin(), resourceDepots.end(), u) == resourceDepots.end()))
		{
			resourceDepots.push_back(u);
		}
	}
}

// Return the wanted base
Unit ProductionManager::getResourceDepot(unsigned int index)
{
	Unit resourceDepot = nullptr;

	if (index < resourceDepots.size())
	{
		resourceDepot = resourceDepots[index];
	}

	return
		resourceDepot;
}

// Construct a building at the specified location
bool ProductionManager::makeBuilding(const UnitType& buildingType, const TilePosition& targetBuildLocation, Unit worker)
{
	bool success = false;

	if (worker != nullptr && targetBuildLocation.isValid())
	{
		success = worker->build(buildingType, targetBuildLocation);
	}

	return 
		success;
}

// Build the wanted unit
bool ProductionManager::makeUnit(int resourceDepotIndex, const UnitType& unitType)
{
	Unit resourceDepot = getResourceDepot(resourceDepotIndex);
	bool success = false;

	if (resourceDepot->getType().isResourceDepot() && resourceDepot->isIdle())
	{
		success = resourceDepot->train(unitType);
	}

	return 
		success;
}

// Tell if we are about to saturate our supply
bool ProductionManager::isSupplyAboutToBeFull()
{
	return 
		Broodwar->self()->supplyUsed() >= (
			Broodwar->self()->supplyTotal() - 
			(SUPPLY_FULL_CHECK_THRESHOLD * UnitTypes::Zerg_Drone.supplyRequired())
		);
}

// Tell if we are using all of our supplies or not
bool ProductionManager::isSupplyFull()
{
	return 
		Broodwar->self()->supplyUsed() >= Broodwar->self()->supplyTotal();
}

// Get number of supply used
int ProductionManager::realSupplyUsed()
{
	return 
		Broodwar->self()->supplyUsed() / 2;
}

// Get max supply available
int ProductionManager::maxSupply()
{
	return
		Broodwar->self()->supplyTotal() / 2;
}

// Tell if a unit is being built or not
bool ProductionManager::isUnitBeingCreated(UnitType unitType)
{
	bool beingCreated = false;

	for (Unit ourUnit : Broodwar->self()->getUnits())
	{
		if (ourUnit->getType() == unitType && !ourUnit->isCompleted())
		{
			beingCreated = true;
			break;
		}
	}

	return 
		beingCreated;
}

// Return the closest unit of the given type using the base at the given index
Unit ProductionManager::getClosestUnit(int resourceDepotIndex, UnitType unitType, int distance)
{
	return 
		getResourceDepot(resourceDepotIndex)->getClosestUnit(GetType == unitType && Exists, distance);
}

// Return the quantity of mineral possessed by the player
int ProductionManager::getMineralCount()
{
	return 
		Broodwar->self()->minerals();
}

// Return the quantity of vespene gas possessed by the player
int ProductionManager::getVespeneCount()
{
	return 
		Broodwar->self()->gas();
}

// Return the closest buildable position for a given building using the position given
TilePosition ProductionManager::getClosestBuildablePosition(UnitType buildingType, TilePosition wantedPosition, int distance)
{
	bool creepNeeded = (buildingType != UnitTypes::Zerg_Hatchery && buildingType != UnitTypes::Zerg_Extractor);

	return 
		Broodwar->getBuildLocation(buildingType, wantedPosition, distance, creepNeeded);
}

// Tell if the player meets the requirements to build/make the given unit
bool ProductionManager::hasUnitRequirements(UnitType unit)
{
	bool requirementsMet = true;
	auto requirements = unit.requiredUnits();

	for (auto &r : requirements)
	{
		requirementsMet &= isAlreadyBuilt(r.first, true);

		if (!requirementsMet)
		{
			break;
		}
	}

	return 
		requirementsMet;
}

// Tell if the player is building something
bool ProductionManager::checkForStartedConstruction()
{
	bool buildingSomething = false;

	// for each building unit which is being constructed
	for (auto &buildingStarted : Broodwar->self()->getUnits()) {
		// filter out units which aren't buildings under construction
		if (!(buildingStarted->getType().isBuilding() && (buildingStarted->isBeingConstructed() || buildingStarted->isMorphing())))
		{
			continue;
		}
		else
		{
			buildingSomething = true;
			break;
		}
	}

	return 
		buildingSomething;
}

// Tell if the given building has already been built 
bool ProductionManager::isAlreadyBuilt(UnitType type, bool completed, int count)
{
	bool specialCases = (type == UnitTypes::Zerg_Lair || type == UnitTypes::Zerg_Hive || type == UnitTypes::Zerg_Greater_Spire);
	bool buildingToCount = false;
	bool isBuilt = false;

	for (auto &b : Broodwar->self()->getUnits())
	{
		buildingToCount = (specialCases) ? b->getType() == type : ToolBox::areUnitsLinkedByEvolution(b->getType(), type);

		if (buildingToCount && (!completed || (completed && b->isCompleted())))
		{
			count--;

			if (count == 0)
			{
				isBuilt = true;
				break;
			}
		}
	}

	return
		isBuilt;
}

// Tell if the resourceDepot at the given index is available to train
bool ProductionManager::canResourceDepotTrain(int index)
{
	Unit depot = getResourceDepot(index);
	bool canTrain = false;

	if (depot != nullptr)
	{
		canTrain = depot->canTrain();
	}

	return 
		canTrain;
}

// Get the number of resource depots
int ProductionManager::getNbResourceDepots()
{
	return 
		resourceDepots.size();
}

// Set the location of all potential bases
void ProductionManager::setAllBaseLocations(set<Base*> newAllBaseLocations)
{
	for (Base* b : newAllBaseLocations)
	{
		allBaseLocations.insert(b);
	}
}

// Return the expansion corresponding to the given index if ordered by ascending distance from main base
Base* ProductionManager::getExpansionOrderedByDistance(int index)
{
	vector<float> sortedBaseDistances;
	Base* expansion = nullptr;

	//Get distance of every non enemy location from main base
	for (Base* b : allBaseLocations)
	{
		expansion = b;
		if (!b->isEnnemyLocation && !b->isInvalidToGroundUnits)
		{
			sortedBaseDistances.push_back((float)b->distanceToMainBase);
		}
	}

	//Sort the list by ascending distances
	sort(sortedBaseDistances.begin(), sortedBaseDistances.end());

	//Get the base corresponding to the wanted distance
	if (index < sortedBaseDistances.size())
	{
		for (Base* b : allBaseLocations)
		{
			if (b->distanceToMainBase == sortedBaseDistances[index])
			{
				expansion = b;
				break;
			}
		}
	}

	return 
		expansion;
}

// Return the expansion that needs to be checked the most
Base* ProductionManager::getMostNeededExpansionToCheck()
{
	vector<float> sortedBaseTimes;
	Base* expansion = nullptr;

	//Get frame check of every location
	for (Base* b : allBaseLocations)
	{
		if (b->hasToBeChecked() && !b->isInvalidToGroundUnits)
		{
			sortedBaseTimes.push_back((float)b->lastFrameChecked);
		}
	}

	//Sort the list by ascending times
	sort(sortedBaseTimes.begin(), sortedBaseTimes.end());

	//Get the base that has to be checked the most
	for (Base* b : allBaseLocations)
	{
		if (b->lastFrameChecked == sortedBaseTimes[sortedBaseTimes.size()-1])
		{
			expansion = b;
			break;
		}
	}

	return 
		expansion;
}

// Return the closest building of the given type from the given position
Unit ProductionManager::getBuildingOfType(UnitType wantedBuilding, Position targetPosition)
{
	Unit building = nullptr;
	UnitType alternateBuilding0 = UnitTypes::None;
	UnitType alternateBuilding1 = UnitTypes::None;
	int resourceDepotIndex = -1;

	if (wantedBuilding == UnitTypes::Zerg_Hatchery)
	{
		alternateBuilding0 = UnitTypes::Zerg_Lair;
		alternateBuilding1 = UnitTypes::Zerg_Hive;
	}
	else if (wantedBuilding == UnitTypes::Zerg_Lair)
	{
		alternateBuilding0 = UnitTypes::Zerg_Hive;
	}
	else if (wantedBuilding == UnitTypes::Zerg_Spire)
	{
		alternateBuilding0 = UnitTypes::Zerg_Greater_Spire;
	}

	for (int i = 0; i < resourceDepots.size(); i++)
	{
		Unit tmpDepot = getResourceDepot(i);

		if ((tmpDepot->isCompleted() || tmpDepot->getType() == UnitTypes::Zerg_Lair || tmpDepot->getType() == UnitTypes::Zerg_Hive) &&
			ToolBox::IsInCircle(tmpDepot->getPosition(), 10, targetPosition, 300)
			)
		{
			resourceDepotIndex = i;
			break;
		}
	}

	if (resourceDepotIndex >= 0)
	{
		if (wantedBuilding == getResourceDepot(resourceDepotIndex)->getType())
		{
			building = getResourceDepot(resourceDepotIndex);
		}

		if (building == nullptr)
		{
			building = getClosestUnit(resourceDepotIndex, wantedBuilding);

			if (building == nullptr && alternateBuilding0 != UnitTypes::None)
			{
				building = getClosestUnit(resourceDepotIndex, alternateBuilding0);

				if (building == nullptr && alternateBuilding1 != UnitTypes::None)
				{
					building = getClosestUnit(resourceDepotIndex, alternateBuilding1);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < resourceDepots.size(); i++)
		{
			if (wantedBuilding == getResourceDepot(i)->getType())
			{
				building = getResourceDepot(i);
			}

			if (building == nullptr)
			{
				building = getClosestUnit(i, wantedBuilding);

				if (building == nullptr && alternateBuilding0 != UnitTypes::None)
				{
					building = getClosestUnit(i, alternateBuilding0);

					if (building == nullptr && alternateBuilding1 != UnitTypes::None)
					{
						building = getClosestUnit(i, alternateBuilding1);
					}
				}
			}

			if (building != nullptr)
			{
				break;
			}
		}
	}

	return 
		building;
}

// Tell if the player is researching the given research
bool ProductionManager::isResearching(TechType wantedResearch)
{
	return 
		Broodwar->self()->isResearching(wantedResearch);
}

// Tell if the player is upgrading the given upgrade
bool ProductionManager::isUpgrading(UpgradeType wantedUpgrade)
{
	return 
		Broodwar->self()->isUpgrading(wantedUpgrade);
}

// Tell if the player has already researched the given research
bool ProductionManager::hasResearched(TechType wantedResearch)
{
	return 
		Broodwar->self()->hasResearched(wantedResearch);
}

// Tell if the player has already upgraded the given upgrade
bool ProductionManager::hasUpgraded(UpgradeType wantedUpgrade)
{
	return 
		Broodwar->self()->getUpgradeLevel(wantedUpgrade) == Broodwar->self()->getMaxUpgradeLevel(wantedUpgrade);
}
