#pragma once
#include <BWAPI.h>

class ToolBox
{
	public:
		//Use a UnitType combined to its TilePosition to get the Position corresponding to its center
		static BWAPI::Position ConvertTilePosition(
			const BWAPI::TilePosition tilePosition,
			const BWAPI::UnitType unitType
		);

		//Check if the given unit is available (not building anything and no pending command on it)
		static bool IsWorkerAvailableToBuild(
			const BWAPI::Unit& unit
		);

		//Check if BWAPI already has a command pending to be executed for the specified unit
		static bool IsCommandPendingOnUnit(
			const BWAPI::Unit& unit
		);

		//Tell if the given TilePosition is a valid one (not invalid, unknown or not set)
		static bool IsTilePositionValid(
			const BWAPI::TilePosition tilePosition
		);

		//Tell if the given Position is a valid one (not invalid, unknown or not set)
		static bool IsPositionValid(
			const BWAPI::Position position
		);

		static bool IsInCircle(
			int posX, int posY, int radius, int pos2X, int pos2y, int radius2
		);

		static bool IsInCircle(
			BWAPI::Position p1, int r1, BWAPI::Position p2, int r2
		);

		static bool areUnitsLinkedByEvolution(
			const BWAPI::UnitType unit1,
			const BWAPI::UnitType unit2
		);

		static BWAPI::UnitType getUnitAbleToResearch(
			BWAPI::TechType researchToMake
		);

		static BWAPI::UnitType getUnitAbleToUpgrade(
			BWAPI::UpgradeType upgradeToMake
		);

		static BWAPI::UnitType getPreviousEvolution(
			BWAPI::UnitType currentEvolution
		);

		static const char YELLOW_CHAR = '\x03';
		static const char WHITE_CHAR = '\x04';
		static const char GREY_CHAR = '\x05';
		static const char DARK_RED_CHAR = '\x06';
		static const char BRIGHT_GREEN_CHAR = '\x07';
		static const char BRIGHT_RED_CHAR = '\x08';
		static const char PURPLE_CHAR = '\x10';
		static const char ORANGE_CHAR = '\x11';
		static const char BLACK_CHAR = '\x14';
		static const char BROWN_CHAR = '\x15';
		static const char DARK_GREEN_CHAR = '\x18';
};

