#pragma once
#include <BWAPI.h>
#include "Base.h"
#include "ToolBox.h"
#include "CombatManager.h"
#include "Squad.h"

class ProductionManager
{
	public:
		static const int SUPPLY_FULL_CHECK_THRESHOLD = 3;	// Number of units to substract to supply used to decide if the supply is almost fully used or not
	
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

		// Get number of supply used
		int realSupplyUsed();

		// Get max supply available
		int maxSupply();

		// Tell if a unit is being built or not
		bool isUnitBeingCreated(
			BWAPI::UnitType unitType
		);

		// Return the closest unit of the given type using the base at the given index
		BWAPI::Unit getClosestUnit(
			int resourceDepotIndex, 
			BWAPI::UnitType unitType,
			int distance = 256
		);

		// Return the quantity of mineral possessed by the player
		int getMineralCount();

		// Return the quantity of vespene gas possessed by the player
		int getVespeneCount();

		// Return the closest buildable position for a given building using the position given
		BWAPI::TilePosition getClosestBuildablePosition(
			BWAPI::UnitType buildingType, 
			BWAPI::TilePosition wantedPosition,
			int distance = 64
		);

		// Tell if the player meets the requirements to build/make the given unit
		bool hasUnitRequirements(
			BWAPI::UnitType unit
		);

		// Tell if the player is building something
		bool checkForStartedConstruction();

		// Tell if the given building has already been built 
		bool isAlreadyBuilt(
			BWAPI::UnitType type,
			bool completed = false,
			int count = 1
		);

		// Tell if the resourceDepot at the given index is available to train
		bool canResourceDepotTrain(
			int index
		);

		// Get the number of resource depots
		int getNbResourceDepots();

		// Set the location of all potential bases
		void setAllBaseLocations(
			std::set<Base*> newAllBaseLocations
		);

		// Return the expansion corresponding to the given index if ordered by ascending distance from main base
		Base* getExpansionOrderedByDistance(
			int index
		);

		// Return the expansion that needs to be checked the most
		Base* getMostNeededExpansionToCheck();

		// Return the closest building of the given type from the given position
		BWAPI::Unit getBuildingOfType(
			BWAPI::UnitType wantedBuilding,
			BWAPI::Position targetPosition
		);

		// Tell if the player is researching the given research
		bool isResearching(
			BWAPI::TechType wantedResearch
		);

		// Tell if the player is upgrading the given upgrade
		bool isUpgrading(
			BWAPI::UpgradeType wantedUpgrade
		);

		// Tell if the player has already researched the given research
		bool hasResearched(
			BWAPI::TechType wantedResearch
		);

		// Tell if the player has already upgraded the given upgrade
		bool hasUpgraded(
			BWAPI::UpgradeType wantedUpgrade
		);

	private:
		std::vector<const BWAPI::Unit> resourceDepots;	// Every resource depot
		std::set<Base*> allBaseLocations;				// Every potential base location
};

