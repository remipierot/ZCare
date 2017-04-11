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
	bool makeBuilding(
		const BWAPI::UnitType& buildingType,
		const BWAPI::TilePosition& targetBuildLocation,
		BWAPI::Unit worker
	);

	// Build the wanted unit
	bool makeUnit(
		int resourceDepotIndex,
		const BWAPI::UnitType& unitType
	);

	// Tell if we are about to saturate our supply
	bool isSupplyAboutToBeFull();

	// Tell if we can build new units or not
	bool isSupplyFull();

	int realSupplyUsed();

	int maxSupply();

	// Tell if a unit is being built or not
	bool isUnitBeingCreated();

	// Return the closest unit of the given type using the base at the given index
	BWAPI::Unit getClosestUnit(
		int resourceDepotIndex, 
		BWAPI::UnitType unitType
	);

	// Return the quantity of mineral possessed by the player
	int getMineralCount();

	// Return the quantity of vespene gas possessed by the player
	int getVespeneCount();

	// Return the closest buildable position for a given building using the position given
	BWAPI::TilePosition getClosestBuildablePosition(
		BWAPI::UnitType buildingType, 
		BWAPI::TilePosition wantedPosition
	);

	bool hasUnitRequirements(
		BWAPI::UnitType unit
	);

	bool checkForStartedConstruction();

	bool isAlreadyBuilt(
		BWAPI::UnitType type,
		bool completed = false,
		int count = 1
	);

<<<<<<< HEAD
	//Mineral Fields
	void setMineralFields(std::set<BWAPI::Position> *mineralFields);
	std::set<BWAPI::Position>* getMineralFields();
=======
	bool canResourceDepotTrain(
		int index
	);

	int getNbResourceDepots();
>>>>>>> 6feea932c4d071a9ff45816d1f745295cc88fcab

private:
	std::set<const BWAPI::Unit> resourceDepots;				// Every base
	std::set<BWAPI::Position> *mineralFields;			// All Mineral fields that are not near a base 
};

