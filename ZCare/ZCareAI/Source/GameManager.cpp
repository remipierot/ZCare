#include "GameManager.h"


using namespace std;
using namespace BWAPI;
using namespace Filter;

// Run the game loop
void GameManager::update()
{
	// Scout logic
	//fillStartingLocations();
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

	//DEBUG DES CERCLES 
	for (auto &base : allBaseLocations)
	{
		Color color(0, 25 * base->idBase, 0);
		std::string s = std::to_string(base->idBase);
		char const *pchar = s.c_str();
		Broodwar->drawText(CoordinateType::Map, base->baseLocation.x, base->baseLocation.y, "Base Location");
		for (Resource* posMineral : base->mineralFields)
		{
			BWAPI::Position &positionMineral = posMineral->resourceUnit->getPosition();
			Broodwar->drawCircle(CoordinateType::Map, positionMineral.x, positionMineral.y, 300, color);
			Broodwar->drawText(CoordinateType::Map, positionMineral.x, positionMineral.y, pchar); 
			if (base->isExpansionInteresting)
				Broodwar->drawText(CoordinateType::Map, positionMineral.x, positionMineral.y+20, "Expansion interesting");
		}


		Color color2(0, 0, 25 * base->idBase);
		if (base->gazFields != 0)
		{
			Resource* posGaz = base->gazFields;
			BWAPI::Position positionGaz = posGaz->resourceUnit->getPosition();
			Broodwar->drawCircle(CoordinateType::Map, positionGaz.x, positionGaz.y, 300, color2);
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

	bool isInCircle = false;
	int idBase = 1;
	Base *base;
	for ( BWAPI::Unit tp : Broodwar->getStaticMinerals())
	{
		const TilePosition mineralsTilePosition = tp->getTilePosition();
		if (ToolBox::IsTilePositionValid(mineralsTilePosition))
		{
			const Position mineralsPosition = ToolBox::ConvertTilePosition(mineralsTilePosition, UnitTypes::Resource_Mineral_Field);
			mineralHelper.insert(new Resource(tp));
		}
	}
	
	for (Resource* mineral : mineralHelper)
	{
		if (mineral->idParent == -1)
			{
				base = new Base();
				base->idBase = idBase;

				BWAPI::Unit unit = mineral->resourceUnit;
				BWAPI::Position positionMineral = unit->getPosition(); 
				base->mineralFields.insert(mineral);
				mineral->idParent = idBase;

				for (const Position posUs : allStartLocations)
				{
					if (!isInCircle && ToolBox::IsInCircle(positionMineral.x, positionMineral.y, 300, posUs.x, posUs.y, 300))
						isInCircle = true;
				}

				base->isStartingLocation = isInCircle;
				isInCircle = false;

				for (Resource* mineral2 : mineralHelper)
				{
					if (mineral != mineral2)
					{
						BWAPI::Position positionMineral2 = mineral2->resourceUnit->getPosition();
						if (ToolBox::IsInCircle(positionMineral.x, positionMineral.y, 300, positionMineral2.x, positionMineral2.y, 300) )
						{
							if (mineral2->idParent == -1)
							{
								mineral2->idParent = idBase;
								base->mineralFields.insert(mineral2);
							}
						}
					}
				}
				idBase += 1;
				allBaseLocations.insert(base);
			}	
	}

	////If a geyser is inside a mineral circle, it's an interesting expansion 
	for (auto &base : allBaseLocations)
	{
		bool expansion = false;
		for (const BWAPI::Unit &gazUnit : Broodwar->getStaticGeysers())
		{
			const TilePosition geysersTilePosition = gazUnit->getTilePosition();
			if (ToolBox::IsTilePositionValid(geysersTilePosition) && !expansion)
			{
				const Position geysersPosition = ToolBox::ConvertTilePosition(geysersTilePosition, UnitTypes::Resource_Vespene_Geyser);
				
				for (Resource* mineralU : base->mineralFields)
				{
					BWAPI::Position positionMineral = mineralU->resourceUnit->getPosition();

					if (ToolBox::IsInCircle(geysersPosition.x, geysersPosition.y, 300, positionMineral.x, positionMineral.y, 300))
					{
						base->gazFields = new Resource(gazUnit);
						expansion = true;
						break;
					}
				}
				if (base->isStartingLocation)
					base->isExpansionInteresting = false;
				else base->isExpansionInteresting = expansion;
			}
		}
	}

	//For the base position 
	for (auto &base : allBaseLocations)
	{
		int mineralCount = base->mineralFields.size();
		BWAPI::Position position(0, 0);
		for (Resource* mineralU : base->mineralFields)
		{
			position += ToolBox::ConvertTilePosition(mineralU->resourceUnit->getTilePosition(), UnitTypes::Resource_Mineral_Field);
		}
		if (base->gazFields != 0)
		{
			mineralCount += 1;
			position += ToolBox::ConvertTilePosition(base->gazFields->resourceUnit->getTilePosition(), UnitTypes::Resource_Vespene_Geyser);
		}
		position /= mineralCount;
		base->baseLocation = position;
		
	}
}

void GameManager::initBO()
{
	_BOParser = BOParser(&_BuildOrder);
	fillStartingLocations();
	_ScoutManager.init(&_ProductionManager);
}

void GameManager::drawDebug()
{
	_BuildOrder.drawDebug();
}
