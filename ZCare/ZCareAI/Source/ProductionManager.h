#pragma once
#include <BWAPI.h>
#include "ToolBox.h"

class ProductionManager
{
public:
	static const int SUPPLY_FULL_CHECK_THRESHOLD = 3;
	
	// Fill the base set
	void updateResourceDepots();

	// Return the wanted base
	BWAPI::Unit getResourceDepot(
		unsigned int index
	);

	// Construct a building at the specified location
	void makeBuilding(
		const BWAPI::UnitType& buildingType,
		const BWAPI::TilePosition& targetBuildLocation,
		BWAPI::Unit worker
	);

	// Tell if we are about to saturate our supply
	bool isSupplyAboutToBeFull();

	// Tell if we can build new units or not
	bool isSupplyFull();

	// Tell if a unit is being built or not
	bool isUnitBeingCreated();

	// Return the closest unit of the given type using the base at the given index
	BWAPI::Unit getClosestUnit(int resourceDepotIndex, BWAPI::UnitType unitType);

	// Return the quantity of mineral possessed by the player
	int getMineralCount();

private:
	std::set<const BWAPI::Unit> resourceDepots;				// Every base
};

