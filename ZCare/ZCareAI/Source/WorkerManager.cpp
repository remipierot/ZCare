#include "WorkerManager.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

// Fill the workers set
void WorkerManager::updateWorkers()
{
	for (auto &u : Broodwar->self()->getUnits())
	{
		if (u->getType().isWorker())
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
			w->gather(w->getClosestUnit(IsMineralField || IsRefinery));
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

// Tell if the given worker is currently gathering resources or not
bool WorkerManager::isWorkerBusy(const Unit worker)
{
	return busyWorkers.find(worker) != busyWorkers.end();
}
