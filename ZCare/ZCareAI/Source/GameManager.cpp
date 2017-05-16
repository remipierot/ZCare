#include "GameManager.h"
#include "ForbiddenPlaces.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

// Run the game loop
void GameManager::update()
{
	// Scout logic
	fillStartingLocations();
	_ScoutManager.updateLocationsToScout(enemyStartLocations, otherStartLocations);
	_ScoutManager.updateScouts();
	_ScoutManager.scout();

	// Worker logic
	_WorkerManager.updateWorkers();

	// Production logic
	_ProductionManager.updateResourceDepots();

	// Build order instruction execution
	_BuildOrder.executeNextInstruction(&_WorkerManager, &_ProductionManager);

	// Workers have to work
	_WorkerManager.sendWorkersToWork();
	_WorkerManager.callWorkersBack();

	//Combat Manager update
	_CombatManager.update();

	for (auto& b : allBaseLocations)
	{
		if (Broodwar->isVisible(b->tilePosition))
		{
			b->lastTimeChecked = Broodwar->getFrameCount();
		}
	}
}

// Number of locations to scout (no matter if they already have been or not)
int GameManager::toScoutCount()
{
	return _ScoutManager.toScoutCount();
}

// Number of scouted locations
int GameManager::scoutedCount()
{
	return _ScoutManager.scoutedCount();
}

// Number of unscouted locations
int GameManager::unscoutedCount()
{
	return _ScoutManager.unscoutedCount();
}

// Number of locations being scout
int GameManager::beingScoutCount()
{
	return _ScoutManager.beingScoutCount();
}

// Number of scouts
int GameManager::scoutsCount()
{
	return _ScoutManager.scoutsCount();
}

// Number of scouts currently scouting
int GameManager::busyScoutsCount()
{
	return _ScoutManager.busyScoutsCount();
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

void GameManager::fillBases()
{
	bool isInCircle = false;
	int idBase = 1;
	Base *base;
	Unit mainBase = _ProductionManager.getResourceDepot(0);

	//Get every mineral field
	for (Unit mineral : Broodwar->getStaticMinerals())
	{
		const TilePosition mineralsTilePosition = mineral->getTilePosition();
		if (ToolBox::IsTilePositionValid(mineralsTilePosition))
		{
			const Position mineralsPosition = ToolBox::ConvertTilePosition(mineralsTilePosition, UnitTypes::Resource_Mineral_Field);
			mineralHelper.insert(new Resource(mineral));
		}
	}

	//Build bases using the mineral fields
	for (Resource* mineral : mineralHelper)
	{
		if (mineral->idParent == -1)
		{
			base = new Base();
			base->idBase = idBase;

			Position mineralPosition = mineral->resourceUnit->getPosition();
			base->mineralFields.insert(mineral);
			mineral->idParent = idBase;

			//Check if mineral is attached to one of the starting locations
			for (const Position startPos : allStartLocations)
			{
				if (!isInCircle && ToolBox::IsInCircle(mineralPosition.x, mineralPosition.y, 10, startPos.x, startPos.y, 300))
				{
					isInCircle = true;
					break;
				}
			}

			base->isStartingLocation = isInCircle;
			isInCircle = false;

			//Look for linked minerals
			for (Resource* linkedMineral : mineralHelper)
			{
				if (mineral != linkedMineral)
				{
					Position linkedMineralPosition = linkedMineral->resourceUnit->getPosition();
					if (ToolBox::IsInCircle(mineralPosition.x, mineralPosition.y, 100, linkedMineralPosition.x, linkedMineralPosition.y, 400))
					{
						if (linkedMineral->idParent == -1)
						{
							linkedMineral->idParent = idBase;
							base->mineralFields.insert(linkedMineral);
						}
					}
				}
			}

			idBase++;
			allBaseLocations.insert(base);
		}
	}

	//If a geyser is inside a mineral circle, it's an interesting expansion 
	for (auto &base : allBaseLocations)
	{
		bool expansion = false;

		//Get every geyser
		for (const Unit &gazUnit : Broodwar->getStaticGeysers())
		{
			const TilePosition geyserTilePosition = gazUnit->getTilePosition();

			if (ToolBox::IsTilePositionValid(geyserTilePosition) && !expansion)
			{
				const Position geyserPosition = gazUnit->getPosition();

				for (Resource* mineral : base->mineralFields)
				{
					Position mineralPosition = mineral->resourceUnit->getPosition();

					if (ToolBox::IsInCircle(geyserPosition.x, geyserPosition.y, 100, mineralPosition.x, mineralPosition.y, 400))
					{
						base->gazField = new Resource(gazUnit);
						expansion = true;
						break;
					}
				}
				
				base->isExpansionInteresting = expansion && !base->isStartingLocation;
			}
		}
	}

	//Store positions and tilePositions
	for (auto &base : allBaseLocations)
	{
		base->computePosition();
		base->computeTilePosition();
		base->setDistanceToMainBase(mainBase);
		base->isInvalidToGroundUnits = ForbiddenPlaces::isPositionForbidden(base->position, Broodwar->mapFileName());

		if (base->isStartingLocation && base->distanceToMainBase > 300)
		{
			base->isEnnemyLocation = true;
		}
	}
	
}

void GameManager::initBO()
{
	_BOParser = BOParser(&_BuildOrder);
	_ProductionManager.updateResourceDepots();
	fillStartingLocations();
	fillBases();
	_ScoutManager.init(&_ProductionManager);
	_ProductionManager.setAllBaseLocations(allBaseLocations);
	_CombatManager.setBase(&allBaseLocations);
}

void GameManager::drawDebug()
{
	//First we handle textual debug on screen space
	int x = 460;
	int y = 20;
	char textColor = ' ';

	//FPS debug info
	Broodwar->drawTextScreen(x + 5, y, "%c FPS", ToolBox::WHITE_CHAR);
	y += 10;
	Broodwar->drawTextScreen(x + 10, y, "%c %d", ToolBox::BRIGHT_GREEN_CHAR, Broodwar->getFPS());
	y += 20;

	//Build order debug info
	y = _BuildOrder.drawDebug(x + 5, y);
	y += 20;

	//Bases debug info
	Broodwar->drawTextScreen(x + 5, y, "%c BASES", ToolBox::WHITE_CHAR);
	y += 10;
	Broodwar->drawTextScreen(x + 10, y, "%c ID - POS - DIST - CHECK", ToolBox::WHITE_CHAR);
	y += 10;
	for (auto &base : allBaseLocations)
	{
		textColor = (base->isEnnemyLocation) ? ToolBox::BRIGHT_RED_CHAR :
					(base->isStartingLocation) ? ToolBox::YELLOW_CHAR :
					(base->isInvalidToGroundUnits) ? ToolBox::GREY_CHAR :
					(base->isExpansionInteresting) ? ToolBox::BRIGHT_GREEN_CHAR :
					ToolBox::ORANGE_CHAR;

		Broodwar->drawTextScreen(
			x + 15,
			y,
			"%c %d - [%d, %d] - %d - %d",
			textColor,
			base->idBase,
			base->position.x,
			base->position.y,
			base->distanceToMainBase,
			base->lastTimeChecked
		);

		y += 10;
	}

	//Then we handle textual and visual debug on game space
	Position mainBase = _ProductionManager.getResourceDepot(0)->getPosition();
	for (auto &base : allBaseLocations)
	{
		Color drawColor(0, 0, 0);

		textColor = (base->isEnnemyLocation) ? ToolBox::BRIGHT_RED_CHAR : 
					(base->isStartingLocation) ? ToolBox::YELLOW_CHAR :
					(base->isInvalidToGroundUnits) ? ToolBox::GREY_CHAR :
					(base->isExpansionInteresting) ? ToolBox::BRIGHT_GREEN_CHAR :
					ToolBox::ORANGE_CHAR;

		drawColor = (base->isEnnemyLocation) ? Colors::Red :
					(base->isStartingLocation) ? Colors::Yellow :
					(base->isInvalidToGroundUnits) ? Colors::Grey :
					(base->isExpansionInteresting) ? Colors::Green :
					Colors::Orange;

		string s = to_string(base->idBase);
		char const *pchar = s.c_str();
		string baseLegend = (base->isEnnemyLocation) ? " [ENEMY]" :
							(base->isStartingLocation) ? " [START]" :
							(base->isInvalidToGroundUnits) ? " [UNREACHABLE]" :
							(base->isExpansionInteresting) ? " [HAS GAS]" :
							" [NO GAS]";

		//Draw circle + text with idBase to locate the base in game
		Broodwar->drawTextMap(base->position.x - 23, base->position.y - 45, "%c %s %d", textColor, "Base", base->idBase);
		Broodwar->drawTextMap(base->position.x - 28, base->position.y - 35, "%c %s", textColor, baseLegend.c_str());
		Broodwar->drawCircleMap(base->position, 20, drawColor, true);

		//Draw idBase on each mineral linked to a specific base, only if minerals are visible
		for (Resource* posMineral : base->mineralFields)
		{
			Position &positionMineral = posMineral->resourceUnit->getPosition();
			Broodwar->drawTextMap(positionMineral.x, positionMineral.y, "%c %s", textColor, pchar);
		}

		//Write vespene on the vespene geyser linked to a specific base, only if geyser is visible
		if (base->gazField != 0)
		{
			Position positionGaz = base->gazField->resourceUnit->getPosition();
			Broodwar->drawTextMap(positionGaz.x - 5, positionGaz.y, "%c %s", textColor, "Vespene");
		}

		//Draw lines between current base and the main one
		if (base->distanceToMainBase > 200)
		{
			Broodwar->drawLineMap(mainBase, base->position, drawColor);
		}
	}
}

CombatManager* GameManager::getCombatManager()
{
	return &_CombatManager;
}

ProductionManager* GameManager::getProductionManager()
{
	return &_ProductionManager;
}

set<Base*> GameManager::getAllBases()
{
	return allBaseLocations;
}