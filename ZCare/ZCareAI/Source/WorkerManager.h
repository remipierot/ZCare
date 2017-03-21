#pragma once
#include <BWAPI.h>
#include "ToolBox.h"

class WorkerManager
{
public:
	// Fill the workers set
	void updateWorkers();

	// Create a worker with the given resourceDepot
	void buildWorker(
		const BWAPI::Unit resourceDepot
	);

	// Send the workers gathering resources
	void sendWorkersToWork();

	// Call workers currently carrying resources back to base
	void callWorkersBack();

	// Number of workers
	int workersCount();

	// Number of workers currently gathering resources
	int busyWorkersCount();

	// Return the worker with the lowest hit points
	BWAPI::Unit getWorkerWithLowestLife();

private:
	std::set<const BWAPI::Unit> workers;				// Every worker
	std::set<const BWAPI::Unit> busyWorkers;			// Workers that are currently gathering resources

	// Tell if the given worker is currently gathering resources or not
	bool isWorkerBusy(
		const BWAPI::Unit worker
	);
};

