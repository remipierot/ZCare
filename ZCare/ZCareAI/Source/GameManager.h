#pragma once
#include <BWAPI.h>
#include "ToolBox.h"
#include "ScoutManager.h"
#include "WorkerManager.h"
#include "ProductionManager.h"
#include "StrategyManager.h"
#include "CombatManager.h"
#include "BuildOrder.h"
#include "BOParser.h"
#include "Base.h"

class GameManager
{
	public:
		// Run the game loop
		void update();	

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

		void initBO();

		void drawDebug();

	private:
		ScoutManager _ScoutManager;						// Scouting manager
		WorkerManager _WorkerManager;					// Worker manager
		ProductionManager _ProductionManager;			// Production manager
		StrategyManager _StrategyManager;				// Strategy manager
		CombatManager _CombatManager;					// Combat manager
		BuildOrder _BuildOrder;
		BOParser _BOParser;
		std::set<BWAPI::Position> allStartLocations;	// All the starting positions
		std::set<BWAPI::Position> enemyStartLocations;	// Starting positions occupied by enemies
		std::set<BWAPI::Position> otherStartLocations;	// Starting positions that are not occupied by me (but maybe by enemies)
		std::set<Resource*> mineralHelper;		// List of mineral ressources (temporary)
		std::set<Base*> allBaseLocations;		// List of struct base used to know all base on the map
		BWAPI::Position personalStartLocation;			// My start position


		// Fill all the starting locations data sets
		void fillStartingLocations();
};

