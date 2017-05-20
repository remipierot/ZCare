#pragma once
#include <set>
#include "BWAPI.h"
#include "ToolBox.h"

class Squad
{
	public:
		// Each squad mode
		enum SquadMode{
			ATTACK,
			DEFENSE,
			PATROL
		};

		Squad(
			int squad
		);

		Squad(
			std::set<const BWAPI::Unit> groundUnits,
			std::set<const BWAPI::Unit> aerialUnits
		);

		~Squad();

		// Make a non aggressive move to the position or unit given
		void neutralMove(
			BWAPI::PositionOrUnit target
		);

		// Make an aggressive move (will attack on sight) to the position or unit given
		void aggressiveMove(
			BWAPI::PositionOrUnit target
		);

		// Insert a unit into the squad
		bool insertUnit(
			BWAPI::Unit unit
		);
	
		// Get the size of the squad
		int getSquadSize();

		// Get the squad id
		int getSquadId();

		// Get the squad maximum capacity
		int getSquadCapacity();

		// Get the set of ground units
		std::set<const BWAPI::Unit>* getGroundUnits();

		// Get the set of flying units
		std::set<const BWAPI::Unit>* getAerialUnits();

		// Set the mode of the squad
		void setMode(SquadMode mode);

		// Set the target location
		void setTargetLocation(BWAPI::Position position);

		// Get the mode
		Squad::SquadMode getMode();

		// Get the target location
		BWAPI::Position getTargetLocation();

	private :
		int id;										// ID to identify the squad
		int capacity;								// Maximum capacity of the squad
		int currentSize;							// Curent size of the squad
		std::set<const BWAPI::Unit> groundUnits;	// Set of grounded units
		std::set<const BWAPI::Unit> aerialUnits;	// Set of flying units
		SquadMode mode;								// Mode of the squad (see the enum)
		BWAPI::Position targetLocation;				// Location of the target where to move
};

