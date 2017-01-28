#pragma once
#include <BWAPI.h>
#include "ToolBox.h"

class ScoutManager
{
	public:
		// Set the locations that have to be scout
		void updateLocationsToScout(
			std::set<BWAPI::Position> enemyStartLocations,
			std::set<BWAPI::Position> otherStartLocations
		);

		// Fill the scouts set
		void updateScouts();

		// Send the scouts to unscouted locations (one scout send per function execution)
		void scout();

	private:
		std::set<BWAPI::Position> locationsToScout;		// Locations that can be scout (no matter if they already have been or not)
		std::set<BWAPI::Position> scoutedLocations;		// Locations that have already been scout
		std::set<BWAPI::Position> unscoutedLocations;	// Locations that are not scouted yet
		std::set<BWAPI::Position> locationsBeingScout;	// Locations that are currently being scout (a scout is on its way to the location)
		std::set<const BWAPI::Unit> scouts;				// Every scout
		std::set<const BWAPI::Unit> busyScouts;			// Scouts that are currently scouting

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
};