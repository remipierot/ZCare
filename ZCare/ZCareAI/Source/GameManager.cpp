#include "GameManager.h"
#include "ForbiddenPlaces.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

// Run the game loop
void GameManager::update()
{
	// Update listing of workers
	_WorkerManager.updateWorkers();

	// Update listing of resource depots
	_ProductionManager.updateResourceDepots();

	// Build order instruction execution
	_BuildOrder.executeNextInstruction(&_WorkerManager, &_ProductionManager);

	// Workers have to work
	_WorkerManager.sendWorkersToWork();
	_WorkerManager.callWorkersBack();

	// Attacking units management
	_CombatManager.update();
	_CombatManager.setDiscoveredUnits(&visibleEnemyUnits);

	//Bases update
	updateBasesFrameCheck();
}

// Load the build order
void GameManager::initBuildOrder()
{
	_BOParser = BOParser(&_BuildOrder);
	_ProductionManager.updateResourceDepots();
	fillBases();
	_ProductionManager.setAllBaseLocations(allPotentialBases);
	_CombatManager.setBase(&allPotentialBases);
}

// Draw the debug of each manager
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
	for (auto &base : allPotentialBases)
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
			base->getPosition().x,
			base->getPosition().y,
			base->distanceToMainBase,
			base->lastFrameChecked
			);

		y += 10;
	}

	//Then we handle textual and visual debug on game space
	Position mainBase = _ProductionManager.getResourceDepot(0)->getPosition();
	for (auto &base : allPotentialBases)
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

		//Draw circle + text to locate the base in game
		Broodwar->drawTextMap(base->getPosition().x - 23, base->getPosition().y - 45, "%c %s %d", textColor, "Base", base->idBase);
		Broodwar->drawTextMap(base->getPosition().x - 28, base->getPosition().y - 35, "%c %s", textColor, baseLegend.c_str());
		Broodwar->drawCircleMap(base->getPosition(), 20, drawColor, true);

		//Draw idBase on each mineral linked to a specific base, only if minerals are visible
		for (Resource* posMineral : base->getMineralFields())
		{
			Position &mineralPosition = posMineral->resourceUnit->getPosition();
			Broodwar->drawTextMap(mineralPosition.x, mineralPosition.y, "%c %s", textColor, pchar);
		}

		//Write vespene on the vespene geyser linked to a specific base, only if geyser is visible
		if (base->getGeyser() != 0)
		{
			Position geyserPosition = base->getGeyser()->resourceUnit->getPosition();
			Broodwar->drawTextMap(geyserPosition.x - 5, geyserPosition.y, "%c %s", textColor, "Vespene");
		}

		//Draw lines between current base and the main one
		if (base->distanceToMainBase > 200)
		{
			Broodwar->drawLineMap(mainBase, base->getPosition(), drawColor);
		}
	}
}

// Used to propagate the onUnitEvade BWAPI callback
void GameManager::onUnitEvade(Unit unit)
{
	bool notOurUnit = true;
	if (!unit->getType().isNeutral())
	{
		for (Unit ourUnit : Broodwar->self()->getUnits())
		{
			if (ourUnit == unit)
			{
				notOurUnit = false;
				break;
			}

		}
		if (notOurUnit)
			visibleEnemyUnits.erase(unit);
	}
}

// Used to propagate the onUnitShow BWAPI callback
void GameManager::onUnitShow(Unit unit)
{
	bool notOurUnit = true;
	if (!unit->getType().isNeutral())
	{
		if (unit->exists())
		{
			for (Unit ourUnit : Broodwar->self()->getUnits())
			{
				if (ourUnit == unit)
				{
					notOurUnit = false;
					break;
				}

			}

			if (notOurUnit)
				visibleEnemyUnits.insert(unit);
		}
	}
}

// Used to propagate the onUnitComplete BWAPI callback
void GameManager::onUnitComplete(Unit unit)
{
	UnitType type = unit->getType();
	if (!type.isWorker() && !type.isBuilding() && unit->canAttack())
	{
		int squadNumber = _CombatManager.squadNumber();
		Squad* squad = 0;
		if (squadNumber == 0)
		{
			squadNumber = 1;
			squad = new Squad(squadNumber);
			squad->setModeSquad(Squad::attackMode);
			Position pos;

			for (Base *base : *_CombatManager.getBase())
			{
				if (base->isEnnemyLocation && base->isStartingLocation)
				{
					pos = base->getPosition();
					break;
				}
			}
			squad->setPositionObjective(pos);
			_CombatManager.addSquad(squad);
		}

		bool insertion = false;
		int currentSquad = 1;
		while (!insertion)
		{
			squad = _CombatManager.findSquad(currentSquad);
			if (squad != 0)
			{
				if (squad->insertUnit(unit))
				{
					insertion = true;
				}
				else currentSquad += 1;
			}

			else
			{
				squad = new Squad(currentSquad);
				squad->setModeSquad(Squad::attackMode);
				Position pos;
				for (Base *base : *_CombatManager.getBase())
				{
					if (base->isEnnemyLocation && base->isStartingLocation)
					{
						pos = base->getPosition();
						break;
					}
				}
				squad->setPositionObjective(pos);
				_CombatManager.addSquad(squad);
			}
		}
	}
}

// Fill the bases data set
void GameManager::fillBases()
{
	bool isInCircle = false;
	int idBase = 1;
	Base *base;
	Unit mainBase = _ProductionManager.getResourceDepot(0);
	set<Resource*> mineralHelper;
	set<Position> allStartLocations;

	// Store all starting locations
	for (const TilePosition tp : Broodwar->getStartLocations())
	{
		allStartLocations.insert(ToolBox::ConvertTilePosition(tp, UnitTypes::Special_Start_Location));
	}

	// Get every mineral field
	for (Unit mineral : Broodwar->getStaticMinerals())
	{
		const TilePosition mineralsTilePosition = mineral->getTilePosition();
		if (ToolBox::IsTilePositionValid(mineralsTilePosition))
		{
			const Position mineralsPosition = ToolBox::ConvertTilePosition(mineralsTilePosition, UnitTypes::Resource_Mineral_Field);
			mineralHelper.insert(new Resource(mineral));
		}
	}

	// Build bases data set using the mineral fields
	for (Resource* mineral : mineralHelper)
	{
		if (mineral->idParent == -1)
		{
			base = new Base();
			base->idBase = idBase;

			Position mineralPosition = mineral->resourceUnit->getPosition();
			base->insertMineral(mineral);
			mineral->idParent = idBase;

			// Check if mineral is attached to one of the starting locations
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

			// Look for minerals linked to the same location
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
							base->insertMineral(linkedMineral);
						}
					}
				}
			}

			idBase++;
			allPotentialBases.insert(base);
		}
	}

	// If a geyser is inside a mineral circle, it's an interesting expansion because it has gas 
	for (auto &base : allPotentialBases)
	{
		bool hasGas = false;

		// Get every geyser
		for (const Unit &gasUnit : Broodwar->getStaticGeysers())
		{
			const TilePosition geyserTilePosition = gasUnit->getTilePosition();

			if (ToolBox::IsTilePositionValid(geyserTilePosition) && !hasGas)
			{
				const Position geyserPosition = gasUnit->getPosition();

				for (Resource* mineral : base->getMineralFields())
				{
					Position mineralPosition = mineral->resourceUnit->getPosition();

					if (ToolBox::IsInCircle(geyserPosition.x, geyserPosition.y, 100, mineralPosition.x, mineralPosition.y, 400))
					{
						base->setGeyser(new Resource(gasUnit));
						hasGas = true;
						break;
					}
				}
				
				base->isExpansionInteresting = hasGas && !base->isStartingLocation;
			}
		}
	}

	// Compute positions and tilePositions
	for (auto &base : allPotentialBases)
	{
		base->computePosition();
		base->computeTilePosition();
		base->distanceToMainBase = mainBase->getPosition().getDistance(base->getPosition());
		base->isInvalidToGroundUnits = ForbiddenPlaces::isPositionForbidden(base->getPosition(), Broodwar->mapFileName());

		if (base->isStartingLocation && base->distanceToMainBase > 300)
		{
			base->isEnnemyLocation = true;
		}
	}
	
}

// Update the lastFrameChecked of each base
void GameManager::updateBasesFrameCheck()
{
	for (auto& b : allPotentialBases)
	{
		if (Broodwar->isVisible(b->getTilePosition()))
		{
			b->lastFrameChecked = Broodwar->getFrameCount();
		}
	}
}
