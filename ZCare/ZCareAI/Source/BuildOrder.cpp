#include "BuildOrder.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

// Add an instruction to the instruction set
void BuildOrder::addInstruction(BOInstruction* instruction)
{
	instructionSet.push_back(instruction);
}

// Get a reference to the next instruction to execute
BOInstruction* BuildOrder::getNextInstruction()
{
	BOInstruction* boi = new BOInstruction(BOInstruction::END_OF_BO);

	if (currentInstruction + 1 < instructionSet.size())
	{
		boi = instructionSet[currentInstruction + 1];
	}

	return 
		boi;
}

// Reset the build order (return to the first instruction to execute)
void BuildOrder::reset()
{
	for (auto i : instructionSet)
	{
		i->reset();
	}

	currentInstruction = -1;
	builder = nullptr;
	tileBuildLocation = TilePositions::None;
}

// Execute the next instruction
bool BuildOrder::executeNextInstruction(WorkerManager* wm, ProductionManager* pm)
{
	bool conditionsFulfilled = false;
	bool executed = false;
	BOInstruction* nextInstruction = getNextInstruction();

	// Run next instruction only if it is not completed
	if (!nextInstruction->isCompleted())
	{
		// Define conditions to fulfill according to instruction type
		switch (nextInstruction->getType())
		{
			case BOInstruction::SUPPLY_USED:
				conditionsFulfilled = true;
				break;
			case BOInstruction::BUILDING_COMPLETION:
				break;
			case BOInstruction::RESOURCE_CAP:
				if (nextInstruction->getMineralCap() > 0)
				{
					conditionsFulfilled = pm->getMineralCount() >= nextInstruction->getMineralCap();
				}
				else if (nextInstruction->getVespeneCap() > 0)
				{
					conditionsFulfilled = pm->getVespeneCount() >= nextInstruction->getVespeneCap();
				}
				break;
			case BOInstruction::END_OF_BO:
				break;
		}

		// Run nextInstruction only if its conditions are fulfilled
		if (conditionsFulfilled)
		{
			int mineralCount = pm->getMineralCount();
			int vespeneCount = pm->getVespeneCount();

			Base* b = pm->getExpansionOrderedByDistance(nextInstruction->getBaseIndex());
			TilePosition targetTilePosition = b->getTilePosition();
			Position targetPosition = b->getPosition();
			Unit targetResourceDepot = nullptr;

			for (int i = 0; i < pm->getNbResourceDepots(); i++)
			{
				Unit tmpDepot = pm->getResourceDepot(i);

				if ((tmpDepot->isCompleted() || tmpDepot->getType() == UnitTypes::Zerg_Lair || tmpDepot->getType() == UnitTypes::Zerg_Hive) &&
					tmpDepot->canTrain() && 
					ToolBox::IsInCircle(tmpDepot->getPosition(), 10, targetPosition, 300)
				)
				{
					targetResourceDepot = tmpDepot;
					targetPosition = tmpDepot->getPosition();
					targetTilePosition = tmpDepot->getTilePosition();
					break;
				}
			}

			// Evolution of buildings
			if (nextInstruction->isEvolution())
			{
				UnitType wantedEvolution = nextInstruction->getUnitToBuild();
				Unit buildingToEvolve = pm->getBuildingOfType(ToolBox::getPreviousEvolution(wantedEvolution), targetPosition);

				if (buildingToEvolve != nullptr)
				{
					buildingToEvolve->morph(wantedEvolution);
				}
			}
			// Research
			else if (nextInstruction->isResearch())
			{
				UnitType buildingType = ToolBox::getUnitAbleToResearch(nextInstruction->getResearch());

				if (buildingType != UnitTypes::None)
				{
					Unit building = pm->getBuildingOfType(buildingType, targetPosition);

					if (building != nullptr)
					{
						building->research(nextInstruction->getResearch());
					}
				}
			}
			// Upgrade
			else if (nextInstruction->isUpgrade())
			{
				UnitType buildingType = ToolBox::getUnitAbleToUpgrade(nextInstruction->getUpgrade());

				if (buildingType != UnitTypes::None)
				{
					Unit building = pm->getBuildingOfType(buildingType, targetPosition);

					if (building != nullptr)
					{
						building->upgrade(nextInstruction->getUpgrade());
					}
				}
			}
			// Make building
			else if (nextInstruction->isBuilding())
			{
				UnitType wantedBuilding = nextInstruction->getUnitToBuild();

				if (builder == nullptr)
				{
					builder = wm->getWorkerWithLowestLife();

					if (targetResourceDepot != nullptr)
					{
						builder = targetResourceDepot->getClosestUnit(GetType == UnitTypes::Zerg_Drone && Exists && !IsConstructing && !IsMorphing, 256);
					}
				}

				if (!ToolBox::IsInCircle(targetPosition, 300, builder->getPosition(), 10))
				{
					if (builder->isGatheringGas() || builder->isGatheringMinerals())
					{
						builder->stop();
					}

					builder->move(targetPosition);
					builder->rightClick(targetPosition);
				}

				Broodwar->drawCircleMap(targetPosition, 300, Colors::Cyan);
				Broodwar->drawCircleMap(builder->getPosition(), 10, Colors::Red, true);

				// Make building if having enough ressources
				if (mineralCount >= wantedBuilding.mineralPrice() && vespeneCount >= wantedBuilding.gasPrice())
				{
					if (tileBuildLocation == TilePositions::None)
					{
						tileBuildLocation = pm->getClosestBuildablePosition(wantedBuilding, targetTilePosition, 15);

						if (!ToolBox::IsTilePositionValid(tileBuildLocation))
						{
							tileBuildLocation = pm->getClosestBuildablePosition(wantedBuilding, targetTilePosition, 30);

							if (!ToolBox::IsTilePositionValid(tileBuildLocation))
							{
								tileBuildLocation = pm->getClosestBuildablePosition(wantedBuilding, targetTilePosition);
							}
						}
					}

					// Specific vespene location
					if (wantedBuilding == UnitTypes::Zerg_Extractor && ToolBox::IsInCircle(targetPosition, 300, builder->getPosition(), 10))
					{
						Unit closestVespene = builder->getClosestUnit(GetType == UnitTypes::Resource_Vespene_Geyser && Exists);

						if (closestVespene != nullptr)
						{
							tileBuildLocation = closestVespene->getTilePosition();
						}
					}

					pm->makeBuilding(wantedBuilding, tileBuildLocation, builder);
					Broodwar->drawCircleMap(builder->getPosition(), 10, Colors::Green, true);
					Broodwar->drawLineMap(builder->getPosition(), ToolBox::ConvertTilePosition(tileBuildLocation, wantedBuilding), Colors::Green);
				}
			}
			// Make unit
			else if (nextInstruction->isUnit())
			{
				UnitType wantedUnit = nextInstruction->getUnitToBuild();

				if (nextInstruction->getNbUnitsToBuild() > 0)
				{
					if (mineralCount >= wantedUnit.mineralPrice() &&
						vespeneCount >= wantedUnit.gasPrice() &&
						pm->hasUnitRequirements(wantedUnit) &&
						(wantedUnit == UnitTypes::Zerg_Overlord || pm->realSupplyUsed() < pm->maxSupply()))
					{
						if (targetResourceDepot != nullptr)
						{
							if (targetResourceDepot->train(wantedUnit))
							{
								nextInstruction->decrementNbUnits();
							}
						}
						else
						{
							for (int i = 0; i < pm->getNbResourceDepots(); i++)
							{
								if ((pm->getResourceDepot(i)->isCompleted() || pm->getResourceDepot(i)->getType() == UnitTypes::Zerg_Lair || pm->getResourceDepot(i)->getType() == UnitTypes::Zerg_Hive) &&
									ToolBox::IsInCircle(pm->getResourceDepot(i)->getPosition(), 10, targetPosition, 300))
								{
									targetResourceDepot = pm->getResourceDepot(i);
									if (targetResourceDepot->train(wantedUnit))
									{
										nextInstruction->decrementNbUnits();
										break;
									}
								}
							}

							if (targetResourceDepot == nullptr && nextInstruction->getUnitToBuild().isWorker())
							{
								if (builder == nullptr)
								{
									builder = wm->getWorkerWithLowestLife();
								}

								builder->move(targetPosition);

								if (ToolBox::IsInCircle(builder->getPosition(), 10, targetPosition, 300))
								{
									nextInstruction->decrementNbUnits();
								}
							}
						}
					}
				}
			}
		}

		if (nextInstruction->isResearch())
		{
			executed = pm->isResearching(nextInstruction->getResearch()) || pm->hasResearched(nextInstruction->getResearch());
		}
		else if (nextInstruction->isUpgrade())
		{
			executed = pm->isUpgrading(nextInstruction->getUpgrade()) || pm->hasUpgraded(nextInstruction->getUpgrade());
		}
		else if (nextInstruction->isBuilding() || nextInstruction->isEvolution())
		{
			executed = (builder == nullptr && pm->isUnitBeingCreated(nextInstruction->getUnitToBuild())) || (builder != nullptr && (builder->isBeingConstructed() || builder->isMorphing()));
		}
		else if (nextInstruction->isUnit() && nextInstruction->getNbUnitsToBuild() <= 0)
		{
			executed = true;
		}

		if (executed)
		{
			nextInstruction->complete();
			builder = nullptr;
			tileBuildLocation = TilePositions::None;
			currentInstruction++;
		}
	}
	
	return 
		executed;
}

// Draw the debug info of the build order
int BuildOrder::drawDebug(int startingX, int startingY)
{
	BOInstruction* toPrint = NULL;
	char color = ' ';
	string baseLocationInfo = "";
	string unitInfo = "";
	int y = 0;

	Broodwar->drawTextScreen(startingX, startingY, "%c Build Order", ToolBox::WHITE_CHAR);

	for (int i = (currentInstruction + 1); i < (currentInstruction + 6); i++)
	{
		color = (i == currentInstruction + 1) ? ToolBox::BRIGHT_GREEN_CHAR : ToolBox::BRIGHT_RED_CHAR;
		if (i < instructionSet.size())
		{
			toPrint = instructionSet[i];
		}
		else
		{
			toPrint = NULL;
		}

		if (toPrint != NULL)
		{
			int baseIndex = toPrint->getBaseIndex();
			
			if (baseIndex == 0)
				baseLocationInfo = "@MAIN";
			else if (baseIndex == 1)
				baseLocationInfo = '@' + to_string(baseIndex) + "ST EXP";
			else if (baseIndex == 2)
				baseLocationInfo = '@' + to_string(baseIndex) + "ND EXP";
			else if (baseIndex == 3)
				baseLocationInfo = '@' + to_string(baseIndex) + "RD EXP";
			else
				baseLocationInfo = '@' + to_string(baseIndex) + "TH EXP";

			if (toPrint->isResearch())
				unitInfo = toPrint->getResearch().c_str();
			else if (toPrint->isUpgrade())
				unitInfo = toPrint->getUpgrade().c_str();
			else
				unitInfo = toPrint->getUnitToBuild().c_str();

			y = startingY + (i - (currentInstruction + 1) + 1) * 10;

			Broodwar->drawTextScreen(
				startingX + 5,
				y,
				"%c %d - %s %s",
				color,
				toPrint->getSupplyCount(),
				unitInfo.c_str(),
				baseLocationInfo.c_str()
			);
		}
	}

	return 
		y;
}
