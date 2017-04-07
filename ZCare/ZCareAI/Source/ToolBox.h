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

		static const char RED_CHAR = '\x08';
		static const char GREEN_CHAR = '\x07';
		static const char WHITE_CHAR = '\x04';
};

