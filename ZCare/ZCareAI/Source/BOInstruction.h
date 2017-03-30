#pragma once
#include <BWAPI.h>
#include "ToolBox.h"

class BOInstruction
{
public:
	enum InstructionType { NB_WORKERS, BUILDING_COMPLETION, RESOURCE_CAP, END_OF_BO };

	BOInstruction();

	BOInstruction(
		InstructionType newType
	);

	void setWorkerCount(
		int newWorkerCount
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
		BWAPI::TilePosition newBuildLocation = BWAPI::TilePositions::None,
		int newNbUnitsToBuild = 1
	);

	InstructionType getType();

	int getWorkerCount();

	int getMineralCap();

	int getVespeneCap();

	int getBuildingCompletionPercentage();

	BWAPI::UnitType getBuildingToTrack();

	int getNbUnitsToBuild();

	BWAPI::UnitType getUnitToBuild();

	BWAPI::TilePosition getBuildLocation();

	void decrementNbUnits();

	const char* typeToStr();

private:
	InstructionType type;

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

