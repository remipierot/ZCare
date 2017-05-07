#include "ScoutManager.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

void ScoutManager::init(ProductionManager *prodManager)
{
	this->prodManager = prodManager;
	nbScoutMax = 0;
}



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
		if (u->getType() == BWAPI::UnitTypes::Zerg_Overlord && u->exists())
		{
			scouts.insert(u);
		}
	}
	int nbScout = scouts.size();

	if (nbScoutMax <= nbScout)
	{
		nbScoutMax = nbScout;
	}

	if (nbScoutMax - nbScout > 0)
	{
		for (int i = 0; i < nbScoutMax - nbScout; i++)
		{
			prodManager->makeUnit(0, BWAPI::UnitTypes::Zerg_Overlord);
		}
	}
}

// Create a scout with the given resourceDepot
void ScoutManager::buildScout(const BWAPI::Unit resourceDepot)
{
	if (resourceDepot->getType().isResourceDepot() && resourceDepot->isIdle())
	{
		resourceDepot->train(BWAPI::UnitTypes::Zerg_Overlord);
	}
}

// Send the scouts to unscouted locations (one scout sent per function execution)
void ScoutManager::scout()
{
	// If we have scouts and unscouted locations
	if (!scouts.empty() && !unscoutedLocations.empty())
	{
		Unit scout = nullptr;
		int smallestDist = INT_MAX;
		Position scoutTarget = Positions::None;

		// Find a not busy scout that is not standing on a location to scout
		for (const Unit s : scouts)
		{
			if (!isScoutBusy(s) && !isScoutOnALocationToScout(s))
			{
				scout = s;
			}
		}
		
		// If we have a scout available for scouting
		if (scout != nullptr && scout->canMove())
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
		
		// Check if the busy scouts are on a location to scout
		for (const Unit s : scouts)
		{
			if (isScoutBusy(s) && isScoutOnALocationToScout(s))
			{
				busyScouts.erase(s);
				locationsBeingScout.erase(s->getPosition());
				unscoutedLocations.erase(s->getPosition());
				scoutedLocations.insert(s->getPosition());

				break;
			}
		}
	}
}

// Number of locations to scout (no matter if they already have been or not)
int ScoutManager::toScoutCount()
{
	return locationsToScout.size();
}

// Number of scouted locations
int ScoutManager::scoutedCount()
{
	return scoutedLocations.size();
}

// Number of unscouted locations
int ScoutManager::unscoutedCount()
{
	return unscoutedLocations.size();
}

// Number of locations being scout
int ScoutManager::beingScoutCount()
{
	return locationsBeingScout.size();
}

// Number of scouts
int ScoutManager::scoutsCount()
{
	return scouts.size();
}

// Number of scouts currently scouting
int ScoutManager::busyScoutsCount()
{
	return busyScouts.size();
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

// Tell if the given scout is standing on a location to scout
bool ScoutManager::isScoutOnALocationToScout(const Unit scout)
{
	return locationsToScout.find(scout->getPosition()) != locationsToScout.end();
}
