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

	bool executeNextInstruction(WorkerManager wm, ProductionManager pm);

private:
	std::vector<BOInstruction*> instructionSet;
	int currentInstruction = -1;
};

