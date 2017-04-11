#pragma once
#include <BWAPI.h>
#include "ToolBox.h"
#include "ProductionManager.h"

class ScoutManager
{
	public:

		void init(ProductionManager *prodManager);

		// Set the locations that have to be scout
		void updateLocationsToScout(
			std::set<BWAPI::Position> enemyStartLocations,
			std::set<BWAPI::Position> otherStartLocations
		);

		// Fill the scouts set
		void updateScouts();

		// Create a scout with the given resourceDepot
		void buildScout(
			const BWAPI::Unit resourceDepot
		);

		// Send the scouts to unscouted locations (one scout send per function execution)
		void scout();

		// Number of locations to scout (no matter if they already have been or not)
		int toScoutCount();

		// Number of scouted locations
		int scoutedCount();

		// Number of unscouted locations
		int unscoutedCount();

		// Number of locations being scout
		int beingScoutCount();

		// Number of scouts
		int scoutsCount();

		// Number of scouts currently scouting
		int busyScoutsCount();

	private:
		ProductionManager *prodManager;
		int nbScoutMax;

		std::set<BWAPI::Position> locationsToScout;		// Locations that can be scout (no matter if they already have been or not)
		std::set<BWAPI::Position> scoutedLocations;		// Locations that have already been scout
		std::set<BWAPI::Position> unscoutedLocations;	// Locations that are not scouted yet
		std::set<BWAPI::Position> locationsBeingScout;	// Locations that are currently being scout (a scout is on its way to the location)
		std::set<const BWAPI::Unit> scouts;				// Every scout
		std::set<const BWAPI::Unit> busyScouts;			// Scouts that are currently scouting
		std::set<BWAPI::Position> *mineralFields;  //Minerald Field that are not near a base and are not in the way 

		// Add the given position to the locations to scout and update the unscouted locations
		void addToLocationsToScout(
			BWAPI::Position position
		);

		// Tell if the given scout is currently scouting or not
		bool isScoutBusy(
			const BWAPI::Unit scout
		);

		// Tell if the given position is being scout right now or not
		bool isLocationBeingScout(
			const BWAPI::Position position
		);

		// Tell if the given scout is standing on a location to scout
		bool isScoutOnALocationToScout(
			const BWAPI::Unit scout
		);
};