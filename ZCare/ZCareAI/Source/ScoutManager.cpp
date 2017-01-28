#include "ScoutManager.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

// Set the locations that have to be scout
void ScoutManager::updateLocationsToScout(set<Position> enemyStartLocations, set<Position> otherStartLocations)
{
	// If no enemy location is known, set other starting locations to be scout
	if (enemyStartLocations.empty())
	{
		for (const Position p : otherStartLocations)
		{
			addToLocationsToScout(p);
		}
	}
	// Else we set the found enemy locations to be scout
	else
	{
		for (const Position p : enemyStartLocations)
		{
			addToLocationsToScout(p);
		}
	}
}

// Fill the scouts set
void ScoutManager::updateScouts()
{
	scouts.clear();

	for (auto &u : Broodwar->self()->getUnits())
	{
		if (u->getType() == BWAPI::UnitTypes::Zerg_Overlord)
		{
			scouts.insert(u);
		}
	}
}

// Send the scouts to unscouted locations (one scout send per function execution)
void ScoutManager::scout()
{
	// If we have scouts and unscouted locations
	if (!scouts.empty() && !unscoutedLocations.empty())
	{
		Unit scout = nullptr;
		int smallestDist = INT_MAX;
		Position scoutTarget = Positions::None;

		// Find the first scout that is not busy
		for (const Unit s : scouts)
		{
			if (!isScoutBusy(s))
			{
				scout = s;
				break;
			}
		}
		
		// If we have a scout available for scouting
		if (scout != nullptr)
		{
			// Search the closest unscouted location that is not being scout right now
			for (const BWAPI::Position p : unscoutedLocations)
			{
				if (!isLocationBeingScout(p))
				{
					const int dist = scout->getDistance(p);
					if (dist < smallestDist)
					{
						scoutTarget = p;
						smallestDist = dist;
					}
				}
			}

			// Send the scout
			if (scoutTarget != Positions::None)
			{
				scout->move(scoutTarget);
				locationsBeingScout.insert(scoutTarget);
				busyScouts.insert(scout);
			}
		}
		
		// Check if the busy scouts have arrived at a location that is being scout right now
		for (const Unit s : busyScouts)
		{
			for (const Position p : locationsBeingScout)
			{
				if (s->getPosition() == p)
				{
					busyScouts.erase(s);
					locationsBeingScout.erase(p);
					unscoutedLocations.erase(p);
					scoutedLocations.insert(p);

					break;
				}
			}
		}
	}
}

// Add the given position to the locations to scout and update the unscouted locations
void ScoutManager::addToLocationsToScout(Position position)
{
	const pair<set<Position>::iterator, bool> insertResult = locationsToScout.insert(position);

	/*
	 * If the insertion is successful, it means that 
	 * the position was not considered as a location to scout
	 * before. So we add it to the unscouted locations as well.
	 */
	if (insertResult.second == true)
	{
		unscoutedLocations.insert(position);
	}
}

// Tell if the given scout is currently scouting or not
bool ScoutManager::isScoutBusy(const Unit scout)
{
	return busyScouts.find(scout) != busyScouts.end();
}

// Tell if the given position is being scout right now or not
bool ScoutManager::isLocationBeingScout(const Position position)
{
	return locationsBeingScout.find(position) != locationsBeingScout.end();
}
