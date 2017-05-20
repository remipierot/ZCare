#pragma once
#include <BWAPI.h>
#include "ToolBox.h"
#include "Squad.h"
#include "Base.h"

class CombatManager
{
	public:
		CombatManager();

		// Manage squads (send them to attack, make them patrol, defend)
		void update();

		// Add a new squad to the squad set
		void addSquad(
			Squad* squad
		);

		// Get the squad with the given id
		Squad* findSquad(
			int id
		);

		// Get the number of squads
		int numberOfSquad();

		// Get the set of squads
		std::set<Squad*> getSquadList();

		// Make the given squad defend its position
		void defensiveMode(
			Squad* squad
		);

		// Make the given squad attack its target 
		void aggressiveMode(
			Squad* squad
		);

		// Make the given squad patrol on its given path
		void patrolMode(
			Squad* squad
		);

		// Store the set of discovered units
		void setDiscoveredUnits(
			std::set<const BWAPI::Unit> *units
		);

		// Store the set of all the base locations
		void setAllBaseLocations(
			std::set<Base*> newAllBaseLocations
		);

		// Draw the debug info of the combat manager
		void drawDebug();

	private:
		std::set<Squad*> squads;						// Set of squads
		std::set<const BWAPI::Unit> *discoveredUnits;	// Set of every discovered unit
		std::set<Base*> allBaseLocations;				// Set of all the base locations
		BWAPI::Unit unitToAttack;						// Target with highest priority

		// Make the given squad attack its target (used by aggressiveMode)
		void attackingLogic(
			std::set<BWAPI::Unit> *erase,
			std::set<const BWAPI::Unit> *unitType,
			Squad *squad,
			bool isGrounded
		);
};

