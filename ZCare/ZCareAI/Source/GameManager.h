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

		// Load the build order
		void initBuildOrder();

		// Draw the debug of each manager
		void drawDebug();

		// Used to propagate the onUnitComplete BWAPI callback
		void onUnitComplete(BWAPI::Unit unit);

		// Used to propagate the onUnitEvade BWAPI callback
		void onUnitEvade(BWAPI::Unit unit);

		// Used to propagate the onUnitShow BWAPI callback
		void onUnitShow(BWAPI::Unit unit);

	private:
		ScoutManager _ScoutManager;						// Scouting manager
		WorkerManager _WorkerManager;					// Worker manager
		ProductionManager _ProductionManager;			// Production manager
		StrategyManager _StrategyManager;				// Strategy manager
		CombatManager _CombatManager;					// Combat manager
		BuildOrder _BuildOrder;							// Store instructions and handle build order execution
		BOParser _BOParser;								// Used to read and load the build order from an external file
		std::set<BWAPI::Position> allStartLocations;	// All the starting positions
		std::set<BWAPI::Position> enemyStartLocations;	// Starting positions occupied by enemies
		std::set<BWAPI::Position> otherStartLocations;	// Starting positions that are not occupied by me (but maybe by enemies)
		std::set<Resource*> mineralHelper;				// List of mineral ressources (temporary)
		std::set<Base*> allBaseLocations;				// List of struct base used to know all base on the map
		std::set<const BWAPI::Unit> visibleEnemyUnits;	//Set of visible enemy units
		BWAPI::Position personalStartLocation;			// My start position


		// Fill all the starting locations data sets
		void fillStartingLocations();

		// Fill the bases data set
		void fillBases();
};

