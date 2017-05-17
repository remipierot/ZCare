#pragma once
#include <BWAPI.h>
#include "ToolBox.h"

class BOInstruction
{
	public:
		enum InstructionType { SUPPLY_USED, BUILDING_COMPLETION, RESOURCE_CAP, END_OF_BO };

		BOInstruction();

		BOInstruction(
			InstructionType newType
		);

		void setSupplyCount(
			int newSupplyCount
		);

		void setMineralCap(
			int newMineralCap
		);

		void setVespeneCap(
			int newVespeneCap
		);

		void setBuildingCompletionInfos(
			int newPercentage,
			BWAPI::UnitType newBuildingToTrack
		);

		void setActions(
			BWAPI::UnitType newUnitToBuild,
			int newNbUnitsToBuild = 1
		);

		void setResearch(
			BWAPI::TechType newResearchToMake
		);

		void setUpgrade(
			BWAPI::UpgradeType newUpgradeToMake
		);

		void setBaseIndex(
			int baseLocationId
		);

		void setNbUnitsOfType(
			int newNbUnitsOfType
		);

		InstructionType getType();

		int getSupplyCount();

		int getMineralCap();

		int getVespeneCap();

		int getBuildingCompletionPercentage();

		BWAPI::UnitType getBuildingToTrack();

		int getNbUnitsToBuild();

		BWAPI::UnitType getUnitToBuild();

		BWAPI::TilePosition getBuildLocation();

		BWAPI::TechType getResearch();

		BWAPI::UpgradeType getUpgrade();

		int getBaseIndex();

		int getNbUnitsOfType();

		void decrementNbUnits();

		void reset();

		void complete();

		bool isCompleted();

		bool isResearch();

		bool isUpgrade();

		bool isEvolution();

		bool isBuilding();

		bool isUnit();

		static InstructionType strToType(
			const char* str
		);

	private:
		InstructionType type;
		bool completed = false;

		// Conditions to fulfill
		int supplyCount;
		int mineralCap;
		int vespeneCap;
		int buildingCompletionPercentage;
		BWAPI::UnitType buildingToTrack;

		// Actions to perform
		int nbUnitsToBuild;
		int baseIndex;
		BWAPI::UnitType unitToBuild;
		BWAPI::TilePosition buildLocation;
		BWAPI::TechType researchToMake;
		BWAPI::UpgradeType upgradeToMake;

		// Additional infos
		int nbUnitsOfType;
};

