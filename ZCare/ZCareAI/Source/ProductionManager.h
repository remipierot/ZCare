#pragma once
#include <BWAPI.h>
#include "ToolBox.h"

class ProductionManager
{
public:
	static const int SUPPLY_FULL_CHECK_THRESHOLD = 3;
	
	// Fill the base set
	void updateResourceDepots();

	// Return the wanted base
	BWAPI::Unit getResourceDepot(unsigned int index);

	// Tell if we are about to saturate our supply
	bool isSupplyAboutToBeFull();

	// Tell if we can build new units or not
	bool isSupplyFull();

	// Tell if a unit is being built or not
	bool isUnitBeingCreated();

private:
	std::set<const BWAPI::Unit> resourceDepots;				// Every base
};

