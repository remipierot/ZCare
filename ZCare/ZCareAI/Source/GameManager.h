#pragma once
#include <BWAPI.h>
#include "ToolBox.h"
#include "ScoutManager.h"

class GameManager
{
	public:
		// Run the game loop
		void update();	

	private:
		ScoutManager _ScoutManager;						// Scouting manager
		std::set<BWAPI::Position> allStartLocations;	// All the starting positions
		std::set<BWAPI::Position> enemyStartLocations;	// Starting positions occupied by enemies
		std::set<BWAPI::Position> otherStartLocations;	// Starting positions that are not occupied by me (but maybe by enemies)
		BWAPI::Position personalStartLocation;			// My start position

		// Fill all the starting locations data sets
		void fillStartingLocations();
};

