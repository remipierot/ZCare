#pragma once
#include <BWAPI.h>
#include "ToolBox.h"

class BOInstruction
{
	public:
		// Each instruction type
		enum InstructionType { 
			SUPPLY_USED, 
			BUILDING_COMPLETION, 
			RESOURCE_CAP, 
			END_OF_BO 
		};

		BOInstruction();

		BOInstruction(
			InstructionType newType
		);

		//********** INSTRUCTION MANAGEMENT **********//
		// Get the type
		InstructionType getType();

		// Remove 1 unit to the nbUnitsToBuild count
		void decrementNbUnits();

		// Set the instruction as not completed
		void reset();

		// Set the instruction as completed
		void complete();

		// Tell if the instruction is completed
		bool isCompleted();

		// Tell if the instruction is about making a research
		bool isResearch();

		// Tell if the instruction is about making an upgrade
		bool isUpgrade();

		// Tell if the instruction is about evolving a building
		bool isEvolution();

		// Tell if the instruction is about making a building
		bool isBuilding();

		// Tell if the instruction is about making a unit
		bool isUnit();

		// Convert the given string to an instruction type
		static InstructionType strToType(
			const char* str
		);
		//********** END INSTRUCTION MANAGEMENT **********//

		//********** REQUIREMENTS **********//
		// Store the given supply count as the requirement
		void setSupplyCount(
			int newSupplyCount
		);

		// Store the given mineral cap as the requirement
		void setMineralCap(
			int newMineralCap
		);

		// Store the given gas cap as the requirement
		void setVespeneCap(
			int newVespeneCap
		);

		// Store the given percentage and building as the requirement
		void setBuildingCompletionInfos(
			int newPercentage,
			BWAPI::UnitType newBuildingToTrack
		);

		// Get the supply count requirement
		int getSupplyCount();

		// Get the mineral count requirement
		int getMineralCap();

		// Get the gas count requirement
		int getVespeneCap();

		// Get the building percentage requirement
		int getBuildingCompletionPercentage();

		// Get the building that needs to be tracked
		BWAPI::UnitType getBuildingToTrack();
		//********** END REQUIREMENTS **********//

		//********** ACTIONS **********//
		// Store the given unit type and its count as the action
		void setUnit(
			BWAPI::UnitType newUnitToBuild,
			int newNbUnitsToBuild = 1
		);

		// Store the given research as the action
		void setResearch(
			BWAPI::TechType newResearchToMake
		);

		// Store the given upgrade as the action
		void setUpgrade(
			BWAPI::UpgradeType newUpgradeToMake
		);

		// Store the given index as the base location where the instruction will be executed
		void setBaseIndex(
			int baseLocationId
		);

		// Get the number of units to build
		int getNbUnitsToBuild();

		// Get the unit type to build
		BWAPI::UnitType getUnitToBuild();

		// Get the research to make
		BWAPI::TechType getResearch();

		// Get the upgrade to make
		BWAPI::UpgradeType getUpgrade();

		// Get the base location index
		int getBaseIndex();
		//********** END ACTIONS **********//

	private:
		//********** INSTRUCTION MANAGEMENT **********//
		InstructionType type;	// Type of the instruction
		bool completed = false;	// True if the instruction has been completed
		//********** END INSTRUCTION MANAGEMENT **********//

		//********** REQUIREMENTS **********//
		int supplyCount;					// Supply count requirement
		int mineralCap;						// Mineral count requirement
		int vespeneCap;						// Gas count requirement
		int buildingCompletionPercentage;	// Building completion requirement
		BWAPI::UnitType buildingToTrack;	// Building that needs to have its completion tracked
		//********** END REQUIREMENTS **********//

		//********** ACTIONS **********//
		int nbUnitsToBuild;					// Number of units to build
		int baseIndex;						// Base location index (0 is main base, 1 is first expansion, etc)
		BWAPI::UnitType unitToBuild;		// Unit that has to be built (living or building)
		BWAPI::TechType researchToMake;		// Research that has to be made
		BWAPI::UpgradeType upgradeToMake;	// Upgrade that has to be made
		//********** END ACTIONS **********//
};

