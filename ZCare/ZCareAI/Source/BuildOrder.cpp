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
	currentInstruction = -1;
}

bool BuildOrder::executeNextInstruction(WorkerManager wm, ProductionManager pm)
{
	bool conditionsFulfilled = false;
	bool executed = false;
	BOInstruction* nextInstruction = getNextInstruction();

	switch (nextInstruction->getType())
	{
		case BOInstruction::NB_WORKERS:
			conditionsFulfilled = wm.workersCount() >= nextInstruction->getWorkerCount();
			break;
		case BOInstruction::BUILDING_COMPLETION:
			break;
		case BOInstruction::RESOURCE_CAP:
			if (nextInstruction->getMineralCap() > 0)
			{
				conditionsFulfilled = pm.getMineralCount() >= nextInstruction->getMineralCap();
			}
			else if (nextInstruction->getVespeneCap() > 0)
			{
				conditionsFulfilled = pm.getVespeneCount() >= nextInstruction->getVespeneCap();
			}
			break;
		case BOInstruction::END_OF_BO:
			break;
	}

	if (conditionsFulfilled)
	{
		UnitType unitToBuild = nextInstruction->getUnitToBuild();

		if (nextInstruction->getUnitToBuild().isBuilding())
		{
			if (builder == nullptr || builder->isMorphing())
			{
				builder = wm.getWorkerWithLowestLife();
			}
			TilePosition buildLocation = pm.getClosestBuildablePosition(unitToBuild, builder->getTilePosition());

			if (unitToBuild == UnitTypes::Zerg_Extractor)
			{
				Unit closestVespene = pm.getClosestUnit(0, UnitTypes::Resource_Vespene_Geyser);;
				
				if (closestVespene != nullptr)
				{
					buildLocation = closestVespene->getTilePosition();
				}
			}

			if (pm.getMineralCount() >= unitToBuild.mineralPrice() && pm.getVespeneCount() >= unitToBuild.gasPrice())
			{
				pm.makeBuilding(unitToBuild, buildLocation, builder);
				executed = builder->isMorphing();
			}
		}
		else
		{
			if (nextInstruction->getNbUnitsToBuild() > 0)
			{
				if (pm.getMineralCount() >= unitToBuild.mineralPrice() && pm.getVespeneCount() >= unitToBuild.gasPrice())
				{
					pm.makeUnit(0, unitToBuild);
					nextInstruction->decrementNbUnits();
				}
			}
			else
			{
				executed = true;
			}
		}
	}
	else
	{
		wm.buildWorker(pm.getResourceDepot(0));
	}

	if (executed)
	{
		currentInstruction++;
	}

	return executed;
}
