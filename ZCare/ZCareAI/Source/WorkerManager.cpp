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

// Create a worker with the given resourceDepot
void WorkerManager::buildWorker(const BWAPI::Unit resourceDepot)
{
	if (resourceDepot->getType().isResourceDepot() && resourceDepot->isIdle())
	{
		resourceDepot->train(resourceDepot->getType().getRace().getWorker());
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

// Number of workers
int WorkerManager::workersCount()
{
	return workers.size();
}

// Number of workers currently gathering resources
int WorkerManager::busyWorkersCount()
{
	return busyWorkers.size();
}

// Return the worker with the lowest hit points
BWAPI::Unit WorkerManager::getWorkerWithLowestLife()
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

	return lowestHPWorker;
}

// Tell if the given worker is currently gathering resources or not
bool WorkerManager::isWorkerBusy(const Unit worker)
{
	return busyWorkers.find(worker) != busyWorkers.end();
}
