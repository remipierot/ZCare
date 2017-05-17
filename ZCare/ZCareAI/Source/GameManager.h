#pragma once
#include <BWAPI.h>
#include "ToolBox.h"
#include "WorkerManager.h"
#include "ProductionManager.h"
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

		// Used to propagate the onUnitEvade BWAPI callback
		void onUnitEvade(
			BWAPI::Unit unit
		);

		// Used to propagate the onUnitShow BWAPI callback
		void onUnitShow(
			BWAPI::Unit unit
		);

		// Used to propagate the onUnitComplete BWAPI callback
		void onUnitComplete(
			BWAPI::Unit unit
		);

	private:
		WorkerManager		_WorkerManager;			// Worker manager
		ProductionManager	_ProductionManager;		// Production manager
		CombatManager		_CombatManager;			// Combat manager
		BuildOrder			_BuildOrder;			// Store instructions and handle build order execution
		BOParser			_BOParser;				// Used to read and load the build order from an external file
		
		std::set<Base*> allPotentialBases;				// All the bases (mineral fields and/or vespene geyser)
		std::set<const BWAPI::Unit> visibleEnemyUnits;	// Set of visible enemy units

		// Fill the bases data set
		void fillBases();

		// Update the lastFrameChecked of each base
		void updateBasesFrameCheck();
};
