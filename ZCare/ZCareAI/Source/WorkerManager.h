#pragma once
#include <BWAPI.h>
#include "ToolBox.h"

class WorkerManager
{
	public:
		// Fill the workers set
		void updateWorkers();

		// Send the workers gathering resources
		void sendWorkersToWork();

		// Call workers currently carrying resources back to base
		void callWorkersBack();

		// Return the worker with the lowest hit points
		BWAPI::Unit getWorkerWithLowestLife();

	private:
		std::set<const BWAPI::Unit> workers;	// Every worker
};

