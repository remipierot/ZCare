#include "GameManager.h"

using namespace BWAPI;

// Run the game loop
void GameManager::update()
{
	fillStartingLocations();

	_ScoutManager.updateLocationsToScout(enemyStartLocations, otherStartLocations);
	_ScoutManager.updateScouts();
	_ScoutManager.scout();
}

// Fill all the starting locations data sets
void GameManager::fillStartingLocations()
{
	// Store all starting locations
	for (const TilePosition tp : Broodwar->getStartLocations())
	{
		allStartLocations.insert(ToolBox::ConvertTilePosition(tp, UnitTypes::Special_Start_Location));

		// Store our starting location
		if (tp == Broodwar->self()->getStartLocation())
		{
			if (!ToolBox::IsPositionValid(personalStartLocation))
			{
				personalStartLocation = ToolBox::ConvertTilePosition(tp, UnitTypes::Special_Start_Location);
			}
		}
		// Store the other starting locations
		else
		{
			otherStartLocations.insert(ToolBox::ConvertTilePosition(tp, UnitTypes::Special_Start_Location));
		}
	}

	// Store enemy starting locations
	for (const Player& p : Broodwar->enemies())
	{
		const TilePosition enemyTilePosition = p->getStartLocation();
		if (ToolBox::IsTilePositionValid(enemyTilePosition))
		{
			const Position enemyPosition = ToolBox::ConvertTilePosition(enemyTilePosition, UnitTypes::Special_Start_Location);
			enemyStartLocations.insert(enemyPosition);
		}
	}
}
