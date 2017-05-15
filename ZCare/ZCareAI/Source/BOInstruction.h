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

	void setNbUnitsOfType(
		int newNbUnitsOfType
	);

	void setBaseIndex(
		int baseLocationId
	);

	InstructionType getType();

	int getSupplyCount();

	int getMineralCap();

	int getVespeneCap();

	int getNbUnitsOfType();

	int getBuildingCompletionPercentage();

	BWAPI::UnitType getBuildingToTrack();

	int getNbUnitsToBuild();

	BWAPI::UnitType getUnitToBuild();

	BWAPI::TilePosition getBuildLocation();

	BWAPI::TechType getResearchToMake();

	BWAPI::UpgradeType getUpgradeToMake();

	int getBaseIndex();

	void decrementNbUnits();

	const char* typeToStr();

	static const char* typeToStr(BOInstruction instruction);

	static InstructionType strToType(const char* str);

	void reset();

	void complete();

	bool isCompleted();

	bool isResearch();

	bool isUpgrade();

	bool isEvolution();

	bool isBuilding();

	bool isUnit();

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

