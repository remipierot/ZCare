#pragma once
#include <BWAPI.h>
#include "ToolBox.h"

class BOInstruction
{
public:
	BOInstruction();
	~BOInstruction();

private:
	// Conditions to fulfill
	int workerCount;
	int mineralCap;
	int vespeneCap;
	int buildingCompletionPercentage;
	BWAPI::UnitType buildingToTrack;

	// Actions to perform
	int nbUnitsToBuild;
	BWAPI::UnitType unitToBuild;
	BWAPI::TilePosition buildLocation;
};

