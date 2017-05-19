#pragma once
#include <BWAPI.h>
#include "ToolBox.h"
#include "BOInstruction.h"
#include "WorkerManager.h"
#include "ProductionManager.h"

class BuildOrder
{
	public:
		// Add an instruction to the instruction set
		void addInstruction(
			BOInstruction* instruction
		);

		// Get a reference to the next instruction to execute
		BOInstruction* getNextInstruction();

		// Reset the build order (return to the first instruction to execute)
		void reset();

		// Execute the next instruction
		bool executeNextInstruction(
			WorkerManager* wm, 
			ProductionManager* pm
		);

		// Draw the debug info of the build order
		int drawDebug(
			int startinX = 10, 
			int startingY = 40
		);

	private:
		std::vector<BOInstruction*> instructionSet;	// List of instructions to execute
		int currentInstruction = -1;				// Current instruction (last executed)
		BWAPI::Unit builder;						// Unit used to make buildings when needed
		BWAPI::TilePosition tileBuildLocation;		// Location where to build
};

