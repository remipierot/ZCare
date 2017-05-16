#pragma once

#include "ToolBox.h"

class ForbiddenPlaces
{
	public:
		static const std::string ASTRAL_BALANCE;			// Astral balance map name
		static const BWAPI::Position ASTRAL_BALANCE_FB_0;	// Astral balance forbidden place 0
		static const BWAPI::Position ASTRAL_BALANCE_FB_1;	// Astral balance forbidden place 1

		// True if the given position can't be reached by grounded units on the given map
		static bool isPositionForbidden(
			BWAPI::Position positionToCheck,
			std::string mapFileName
		);
};