#include "ForbiddenPlaces.h"

using namespace std;
using namespace BWAPI;

const string ForbiddenPlaces::ASTRAL_BALANCE = "Astral Balance";
const Position ForbiddenPlaces::ASTRAL_BALANCE_FB_0 = Position(485, 109);
const Position ForbiddenPlaces::ASTRAL_BALANCE_FB_1 = Position(3610, 2962);

// True if the given position can't be reached by grounded units on the given map
bool ForbiddenPlaces::isPositionForbidden(Position positionToCheck, string mapFileName)
{
	bool isForbidden = false;

	if (mapFileName.find(ASTRAL_BALANCE) != string::npos) 
	{
		isForbidden |= (positionToCheck.x == ASTRAL_BALANCE_FB_0.x && positionToCheck.y == ASTRAL_BALANCE_FB_0.y);
		isForbidden |= (positionToCheck.x == ASTRAL_BALANCE_FB_1.x && positionToCheck.y == ASTRAL_BALANCE_FB_1.y);
	}

	return isForbidden;
}