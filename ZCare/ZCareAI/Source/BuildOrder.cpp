#include "BuildOrder.h"

using namespace std;
using namespace BWAPI;

void BuildOrder::addInstruction(BOInstruction* instruction)
{
	instructionSet.push_back(instruction);
}

BOInstruction* BuildOrder::getNextInstruction()
{
	if (currentInstruction + 1 < instructionSet.size())
	{
		return instructionSet[currentInstruction + 1];
	}
	else
	{
		return new BOInstruction(BOInstruction::END_OF_BO);
	}
}

void BuildOrder::reset()
{
	for (auto i : instructionSet)
	{
		i->reset();
	}

	currentInstruction = -1;
	builder = nullptr;
	builderBuildingToSpot = false;
	builderMovingToSpot = false;
}

bool BuildOrder::executeNextInstruction(WorkerManager* wm, ProductionManager* pm)
{
	bool conditionsFulfilled = false;
	bool executed = false;
	BOInstruction* nextInstruction = getNextInstruction();
	UnitType unitToBuild = nextInstruction->getUnitToBuild();
	int originNbUnitsToBuild = nextInstruction->getNbUnitsToBuild();

	//Run next instruction only if it is not completed
	if (!nextInstruction->isCompleted())
	{
		//Define conditions to fulfill according to instruction type
		switch (nextInstruction->getType())
		{
			case BOInstruction::SUPPLY_USED:
				conditionsFulfilled = true; // pm->realSupplyUsed() >= nextInstruction->getSupplyCount();
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

		//Run nextInstruction only if its conditions are fulfilled
		if (conditionsFulfilled)
		{
			int mineralCount = pm->getMineralCount();
			int vespeneCount = pm->getVespeneCount();

			Base* b = pm->getExpansionOrderedByDistance(nextInstruction->getBaseIndex());
			TilePosition targetTilePosition = b->tilePosition;
			Position targetPosition = b->position;
			Unit targetResourceDepot = nullptr;

			for (int i = 0; i < pm->getNbResourceDepots(); i++)
			{
				if (pm->canResourceDepotTrain(i) && ToolBox::IsInCircle(pm->getResourceDepot(i)->getPosition(), 10, targetPosition, 300))
				{
					targetResourceDepot = pm->getResourceDepot(i);
					break;
				}
			}


			//Evolution of buildings
			if (nextInstruction->isEvolution())
			{
				UnitType wantedEvolution = nextInstruction->getUnitToBuild();
				Unit buildingToEvolve = pm->getBuildingOfType(ToolBox::getPreviousEvolution(wantedEvolution));

				if (buildingToEvolve != nullptr)
				{
					buildingToEvolve->morph(wantedEvolution);
				}
			}
			//Research
			else if (nextInstruction->isResearch())
			{
				UnitType buildingType = ToolBox::getUnitAbleToResearch(nextInstruction->getResearchToMake());

				if (buildingType != UnitTypes::None)
				{
					Unit building = pm->getBuildingOfType(buildingType);

					if (building != nullptr)
					{
						building->research(nextInstruction->getResearchToMake());
					}
				}
			}
			//Upgrade
			else if (nextInstruction->isUpgrade())
			{
				UnitType buildingType = ToolBox::getUnitAbleToUpgrade(nextInstruction->getUpgradeToMake());

				if (buildingType != UnitTypes::None)
				{
					Unit building = pm->getBuildingOfType(buildingType);

					if (building != nullptr)
					{
						building->upgrade(nextInstruction->getUpgradeToMake());
					}
				}
			}
			//Make building
			else if (nextInstruction->isBuilding())
			{
				if (builder == nullptr)
				{
					builder = wm->getWorkerWithLowestLife();
				}

				if (tileBuildLocation == TilePositions::Invalid)
				{
					tileBuildLocation = pm->getClosestBuildablePosition(unitToBuild, targetTilePosition, 50);
				}

				//Specific vespene location
				if (unitToBuild == UnitTypes::Zerg_Extractor)
				{
					Unit closestVespene = pm->getClosestUnit(nextInstruction->getBaseIndex(), UnitTypes::Resource_Vespene_Geyser);

					if (closestVespene != nullptr)
					{
						tileBuildLocation = closestVespene->getTilePosition();
					}
				}

				//Make building if having enough ressources
				if (mineralCount >= unitToBuild.mineralPrice() && vespeneCount >= unitToBuild.gasPrice())
				{
					if (builder->isGatheringGas() || builder->isGatheringMinerals())
					{
						builder->stop();
					}

					if (!builder->isMoving() && !ToolBox::IsInCircle(targetPosition, 300, builder->getPosition(), 10))
					{
						builder->move((Position)tileBuildLocation);
						builderMovingToSpot = true;
					}

					Broodwar->drawCircleMap((Position)tileBuildLocation, 300, Colors::Cyan);
					Broodwar->drawCircleMap(builder->getPosition(), 10, Colors::Red, true);

					Broodwar->drawLineMap((Position)tileBuildLocation, builder->getPosition(), Colors::Red);

					if (ToolBox::IsInCircle(targetPosition, 300, builder->getPosition(), 10))
					{
						pm->makeBuilding(unitToBuild, tileBuildLocation, builder);
						Broodwar->drawCircleMap(builder->getPosition(), 10, Colors::Green, true);
						builderBuildingToSpot = true;
					}
				}
			}
			//Make unit
			else if (nextInstruction->isUnit())
			{
				if (nextInstruction->getNbUnitsToBuild() > 0)
				{
					if (mineralCount >= unitToBuild.mineralPrice() &&
						vespeneCount >= unitToBuild.gasPrice() &&
						pm->hasUnitRequirements(unitToBuild) &&
						(unitToBuild == UnitTypes::Zerg_Overlord || pm->realSupplyUsed() < pm->maxSupply()))
					{
						if (targetResourceDepot->train(unitToBuild))
						{
							nextInstruction->decrementNbUnits();
						}
						else
						{
							for (int i = 0; i < pm->getNbResourceDepots(); i++)
							{
								if (ToolBox::IsInCircle(pm->getResourceDepot(i)->getPosition(), 10, targetPosition, 300))
								{
									targetResourceDepot = pm->getResourceDepot(i);
									if (targetResourceDepot->train(unitToBuild))
									{
										nextInstruction->decrementNbUnits();
										break;
									}
								}
							}
						}
					}
				}
			}
		}

		if (nextInstruction->isResearch())
		{
			executed = pm->isResearching(nextInstruction->getResearchToMake()) || pm->hasResearched(nextInstruction->getResearchToMake());
		}
		else if (nextInstruction->isUpgrade())
		{
			executed = pm->isUpgrading(nextInstruction->getUpgradeToMake()) || pm->hasUpgraded(nextInstruction->getUpgradeToMake());
		}
		else if (nextInstruction->getUnitToBuild().isBuilding())
		{
			executed = pm->isAlreadyBuilt(unitToBuild, false, nextInstruction->getNbUnitsOfType());
		}
		else if (nextInstruction->getNbUnitsToBuild() <= 0)
		{
			executed = true;
		}

		if (executed)
		{
			nextInstruction->complete();
			builder = nullptr;
			builderBuildingToSpot = false;
			builderMovingToSpot = false;
			tileBuildLocation = TilePositions::Invalid;
			currentInstruction++;
		}
	}
	
	return executed;
}

void BuildOrder::drawDebug()
{
	BOInstruction* toPrint = NULL;
	char color = ' ';
	string baseLocationInfo = "";
	string unitInfo = "";

	Broodwar->drawTextScreen(5, 20, "Build Order data");

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
				unitInfo = toPrint->getResearchToMake().c_str();
			else if (toPrint->isUpgrade())
				unitInfo = toPrint->getUpgradeToMake().c_str();
			else
				unitInfo = toPrint->getUnitToBuild().c_str();

			Broodwar->drawTextScreen(
				10,
				20 + (i - (currentInstruction + 1) + 1) * 10,
				"%c %d - %s %s",
				color,
				toPrint->getSupplyCount(),
				unitInfo.c_str(),
				baseLocationInfo.c_str()
			);
		}
	}
}
