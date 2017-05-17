#include "WorkerManager.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

// Fill the workers set
void WorkerManager::updateWorkers()
{
	workers.clear();

	for (auto &u : Broodwar->self()->getUnits())
	{
		if (u->getType().isWorker() && !u->isMorphing())
		{
			workers.insert(u);
		}
	}
}

// Send the workers gathering resources
void WorkerManager::sendWorkersToWork()
{
	for (const Unit w : workers)
	{
		if (w->isIdle())
		{
			Unit closestMineral = w->getClosestUnit(IsMineralField);
			Unit closestRefinery = w->getClosestUnit(IsRefinery);

			if (closestRefinery != nullptr && !closestRefinery->isBeingGathered())
			{
				w->gather(closestRefinery);
			}
			else
			{
				w->gather(closestMineral);
			}
		}
	}
}

// Call workers currently carrying resources back to base
void WorkerManager::callWorkersBack()
{
	for (const Unit w : workers)
	{
		if (w->isIdle() && (w->isCarryingGas() || w->isCarryingMinerals()))
		{
			w->returnCargo();
		}
	}
}

// Return the worker with the lowest hit points
Unit WorkerManager::getWorkerWithLowestLife()
{
	Unit lowestHPWorker = NULL;

	for (const Unit w : workers)
	{
		if (lowestHPWorker == NULL)
		{
			lowestHPWorker = w;
		}

		if (w->getHitPoints() < lowestHPWorker->getHitPoints() && 
			w->canBuild() && 
			!w->isMorphing() && 
			!w->isCarryingGas() && 
			!w->isCarryingMinerals())
		{
			lowestHPWorker = w;
		}
	}

	return 
		lowestHPWorker;
}
