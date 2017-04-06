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

	if (!nextInstruction->isCompleted())
	{
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

		if (conditionsFulfilled)
		{
			int mineralCount = pm->getMineralCount();
			int vespeneCount = pm->getVespeneCount();

			if (nextInstruction->getUnitToBuild().isBuilding())
			{
				builder = wm->getWorkerWithLowestLife();
				TilePosition buildLocation = pm->getClosestBuildablePosition(unitToBuild, builder->getTilePosition());

				//Specific vespene location
				if (unitToBuild == UnitTypes::Zerg_Extractor)
				{
					Unit closestVespene = pm->getClosestUnit(0, UnitTypes::Resource_Vespene_Geyser);

					if (closestVespene != nullptr)
					{
						buildLocation = closestVespene->getTilePosition();
					}
				}

				if (mineralCount >= unitToBuild.mineralPrice() && vespeneCount >= unitToBuild.gasPrice())
				{
					pm->makeBuilding(unitToBuild, buildLocation, builder);
				}
			}
			else
			{
				if (nextInstruction->getNbUnitsToBuild() > 0)
				{
					if (mineralCount >= unitToBuild.mineralPrice() && vespeneCount >= unitToBuild.gasPrice() && pm->hasUnitRequirements(unitToBuild))
					{
						pm->makeUnit(0, unitToBuild);
						nextInstruction->decrementNbUnits();
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
			executed = pm->isAlreadyBuilt(unitToBuild);
		}
		else if (nextInstruction->getNbUnitsToBuild() <= 0)
		{
			executed = true;
		}

		if (executed)
		{
			nextInstruction->complete();
			currentInstruction++;

			Broodwar->sendText("Instruction %d completed (%s) - %d %s built", 
				currentInstruction,
				nextInstruction->typeToStr(), 
				originNbUnitsToBuild,
				nextInstruction->getUnitToBuild().c_str()
			);
		}
	}
	
	return executed;
}
