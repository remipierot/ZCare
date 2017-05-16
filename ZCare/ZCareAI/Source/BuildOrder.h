#pragma once
#include <BWAPI.h>
#include "ToolBox.h"
#include "BOInstruction.h"
#include "WorkerManager.h"
#include "ProductionManager.h"

class BuildOrder
{
public:
	void addInstruction(BOInstruction* instruction);

	BOInstruction* getNextInstruction();

	void reset();

	bool executeNextInstruction(WorkerManager* wm, ProductionManager* pm);

	int drawDebug(int startinX = 10, int startingY = 40);

private:
	std::vector<BOInstruction*> instructionSet;
	int currentInstruction = -1;
	BWAPI::Unit builder;
	bool builderMovingToSpot;
	bool builderBuildingToSpot;
	BWAPI::TilePosition tileBuildLocation;
};

