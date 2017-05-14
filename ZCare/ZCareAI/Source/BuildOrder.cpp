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
				conditionsFulfilled = pm->realSupplyUsed() >= nextInstruction->getSupplyCount();
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

			TilePosition targetLocation = TilePositions::None;
			Position p = Positions::Invalid;
			Unit targetResourceDepot = nullptr;

			if ((nextInstruction->getBaseIndex() + 1) <= pm->getNbResourceDepots())
			{
				targetResourceDepot = pm->getResourceDepot(nextInstruction->getBaseIndex());
				p = targetResourceDepot->getPosition();
				targetLocation = targetResourceDepot->getTilePosition();
			}
			else
			{
				Base* b = pm->getExpansionOrderedByDistance(nextInstruction->getBaseIndex());
				targetLocation = b->tilePosition;
				p = b->position;
			}

			if (nextInstruction->getUnitToBuild() == UnitTypes::Zerg_Lair)
			{
				pm->getResourceDepot(nextInstruction->getBaseIndex())->morph(UnitTypes::Zerg_Lair);
			}
			//If nextInstruction is about making a building
			if (nextInstruction->getUnitToBuild().isBuilding())
			{
				builder = wm->getWorkerWithLowestLife();
				TilePosition buildLocation = pm->getClosestBuildablePosition(unitToBuild, targetLocation);

				//Specific vespene location
				if (unitToBuild == UnitTypes::Zerg_Extractor)
				{
					Unit closestVespene = pm->getClosestUnit(nextInstruction->getBaseIndex(), UnitTypes::Resource_Vespene_Geyser);

					if (closestVespene != nullptr)
					{
						buildLocation = closestVespene->getTilePosition();
					}
				}

				//Make building if having enough ressources
				if (mineralCount >= unitToBuild.mineralPrice() && vespeneCount >= unitToBuild.gasPrice())
				{
					if (!builder->isMoving() && !ToolBox::IsInCircle(p.x, p.y, 300, builder->getPosition().x, builder->getPosition().y, 10))
					{
						builder->move((Position)buildLocation);
					}

					Broodwar->drawCircleMap((Position)buildLocation, 300, Colors::Cyan);
					Broodwar->drawCircleMap(builder->getPosition(), 10, Colors::Red, true);

					if (ToolBox::IsInCircle(p.x, p.y, 300, builder->getPosition().x, builder->getPosition().y, 10))
					{
						pm->makeBuilding(unitToBuild, buildLocation, builder);
						Broodwar->drawCircleMap(builder->getPosition(), 10, Colors::Green, true);
					}
				}
			}
			//If nextInstruction is about making a living unit
			else
			{
				if (nextInstruction->getNbUnitsToBuild() > 0)
				{
					if (mineralCount >= unitToBuild.mineralPrice() && 
						vespeneCount >= unitToBuild.gasPrice() && 
						pm->hasUnitRequirements(unitToBuild) &&
						(unitToBuild == UnitTypes::Zerg_Overlord || pm->realSupplyUsed() < pm->maxSupply()))
					{
						if (targetResourceDepot == nullptr || 
							(targetResourceDepot != nullptr && !targetResourceDepot->canTrain()) ||
							(targetResourceDepot != nullptr && !targetResourceDepot->isIdle()))
						{
							for (int i = 0; i < pm->getNbResourceDepots(); i++)
							{
								if (pm->canResourceDepotTrain(i))
								{
									targetResourceDepot = pm->getResourceDepot(i);
									break;
								}
							}
						}

						if (targetResourceDepot->train(unitToBuild))
						{
							nextInstruction->decrementNbUnits();
						}
					}
				}
			}
		}
		//else if (nextInstruction->getType() == BOInstruction::SUPPLY_USED && pm->realSupplyUsed() < nextInstruction->getSupplyCount())
		//{
		//	wm->buildWorker(pm->getResourceDepot(0));
		//}

		if (nextInstruction->getUnitToBuild().isBuilding())
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
			currentInstruction++;

			/*
			Broodwar->sendText("Instruction %d completed (%s) - %d %s built", 
				currentInstruction,
				nextInstruction->typeToStr(), 
				originNbUnitsToBuild,
				nextInstruction->getUnitToBuild().c_str()
			);
			*/
		}
	}
	
	return executed;
}

void BuildOrder::drawDebug()
{
	BOInstruction* toPrint = NULL;
	char color = ' ';

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
			Broodwar->drawTextScreen(
				10,
				20 + (i - (currentInstruction + 1) + 1) * 10,
				"%c %d - %s",
				color,
				toPrint->getSupplyCount(),
				toPrint->getUnitToBuild().c_str()
			);
		}
	}
}
