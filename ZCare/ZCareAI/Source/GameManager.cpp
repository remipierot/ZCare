#include "GameManager.h"


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
					if (ToolBox::IsInCircle(mineralPosition.x, mineralPosition.y, 10, linkedMineralPosition.x, linkedMineralPosition.y, 300))
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

					if (ToolBox::IsInCircle(geyserPosition.x, geyserPosition.y, 10, mineralPosition.x, mineralPosition.y, 300))
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
		base->setDistanceToMainBase(_ProductionManager.getResourceDepot(0));
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
}

void GameManager::drawDebug()
{
	_BuildOrder.drawDebug();

	//DEBUG DES ressources et bases
	for (auto &base : allBaseLocations)
	{
		Color color(0, 0, 0);

		color = (base->isStartingLocation) ? Colors::Green :
				(base->isEnnemyLocation) ? Colors::Red :
				(base->isExpansionInteresting) ? Colors::Cyan :
				Colors::Blue;

		std::string s = std::to_string(base->idBase);
		char const *pchar = s.c_str();
		Broodwar->drawTextMap(base->position.x - 35, base->position.y - 35, "%c%s", ToolBox::GREEN_CHAR, "Base Location");
		Broodwar->drawCircleMap(base->position, 20, color, true);

		for (Resource* posMineral : base->mineralFields)
		{
			BWAPI::Position &positionMineral = posMineral->resourceUnit->getPosition();
			Broodwar->drawText(CoordinateType::Map, positionMineral.x, positionMineral.y, pchar);
		}

		if (base->gazField != 0)
		{
			Resource* posGaz = base->gazField;
			BWAPI::Position positionGaz = posGaz->resourceUnit->getPosition();
			Broodwar->drawText(CoordinateType::Map, positionGaz.x, positionGaz.y, "%c%s", ToolBox::GREEN_CHAR, "Vespene");
		}
	}
}

CombatManager* GameManager::getCombatManager()
{
	return &_CombatManager;
}