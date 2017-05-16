#pragma once

#include "ToolBox.h"

class ForbiddenPlaces
{
	public:
		static const std::string ASTRAL_BALANCE;
		static const BWAPI::Position ASTRAL_BALANCE_FB_0;
		static const BWAPI::Position ASTRAL_BALANCE_FB_1;

		static bool isPositionForbidden(
			BWAPI::Position positionToCheck,
			std::string mapFileName
		);
};