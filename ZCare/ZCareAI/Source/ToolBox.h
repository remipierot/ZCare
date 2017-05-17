 #pragma once
#include <BWAPI.h>

class ToolBox
{
	public:
		// Use a UnitType combined to its TilePosition to get the Position corresponding to its center
		static BWAPI::Position ConvertTilePosition(
			const BWAPI::TilePosition tilePosition,
			const BWAPI::UnitType unitType
		);

		// Tell if the given TilePosition is a valid one (not invalid, unknown or not set)
		static bool IsTilePositionValid(
			const BWAPI::TilePosition tilePosition
		);

		// Tell if the two wanted circles overlap
		static bool IsInCircle(
			int x1, int y1, int r1, int x2, int y2, int r2
		);

		// Tell if the two wanted circles overlap
		static bool IsInCircle(
			BWAPI::Position p1, int r1, BWAPI::Position p2, int r2
		);

		// Tell if the one of the units is an evolution of the other one
		static bool areUnitsLinkedByEvolution(
			const BWAPI::UnitType unit1,
			const BWAPI::UnitType unit2
		);

		// Return the UnitType able to make the given research
		static BWAPI::UnitType getUnitAbleToResearch(
			BWAPI::TechType researchToMake
		);

		// Return the UnitType able to make the given upgrade
		static BWAPI::UnitType getUnitAbleToUpgrade(
			BWAPI::UpgradeType upgradeToMake
		);

		// Return the previous evolution UnitType of the given UnitType
		static BWAPI::UnitType getPreviousEvolution(
			BWAPI::UnitType currentEvolution
		);

		static const char YELLOW_CHAR = '\x03';										// Prefix to print yellow messages
		static const char WHITE_CHAR = '\x04';										// Prefix to print white messages
		static const char GREY_CHAR = '\x05';										// Prefix to print grey messages
		static const char DARK_RED_CHAR = '\x06';									// Prefix to print dark red messages
		static const char BRIGHT_GREEN_CHAR = '\x07';								// Prefix to print bright green messages
		static const char BRIGHT_RED_CHAR = '\x08';									// Prefix to print bright red messages
		static const char PURPLE_CHAR = '\x10';										// Prefix to print purple messages
		static const char ORANGE_CHAR = '\x11';										// Prefix to print orange messages
		static const char BLACK_CHAR = '\x14';										// Prefix to print black messages
		static const char BROWN_CHAR = '\x15';										// Prefix to print brown messages
		static const char DARK_GREEN_CHAR = '\x18';									// Prefix to print dark green messages
		static const int BASE_FPS = 15;												// Normal framerate of the game
		static const int SEC_UNTIL_CHECK_BASE = 90;									// Seconds to wait before setting a base as needed to be check
		static const int FRAMES_UNTIL_CHECK_BASE = BASE_FPS * SEC_UNTIL_CHECK_BASE;	// Frames to wait before setting a base as needed to be check
};
