#pragma once
#include <BWAPI.h>
#include "ToolBox.h"

class ProductionManager
{
public:
	// Fill the base set
	void updateResourceDepots();

	// Return the wanted base
	BWAPI::Unit getResourceDepot(int index);

private:
	std::set<const BWAPI::Unit> resourceDepots;				// Every base
};

