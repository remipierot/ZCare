#include "BOInstruction.h"

using namespace BWAPI;

BOInstruction::BOInstruction()
{
	type = END_OF_BO;
}

BOInstruction::BOInstruction(InstructionType newType)
{
	type = newType;
}

void BOInstruction::setWorkerCount(int newWorkerCount)
{
	workerCount = newWorkerCount;
}

void BOInstruction::setMineralCap(int newMineralCap)
{
	mineralCap = newMineralCap;
}

void BOInstruction::setVespeneCap(int newVespeneCap)
{
	vespeneCap = newVespeneCap;
}

void BOInstruction::setBuildingCompletionInfos(int newPercentage, UnitType newBuildingToTrack)
{
	buildingCompletionPercentage = newPercentage;
	buildingToTrack = newBuildingToTrack;
}

void BOInstruction::setActions(UnitType newUnitToBuild, int newNbUnitsToBuild)
{
	unitToBuild = newUnitToBuild;
	nbUnitsToBuild = newNbUnitsToBuild;
}

BOInstruction::InstructionType BOInstruction::getType()
{
	return type;
}

int BOInstruction::getWorkerCount()
{
	return workerCount;
}

int BOInstruction::getMineralCap()
{
	return mineralCap;
}

int BOInstruction::getVespeneCap()
{
	return vespeneCap;
}

int BOInstruction::getBuildingCompletionPercentage()
{
	return buildingCompletionPercentage;
}

UnitType BOInstruction::getBuildingToTrack()
{
	return buildingToTrack;
}

int BOInstruction::getNbUnitsToBuild()
{
	return nbUnitsToBuild;
}

UnitType BOInstruction::getUnitToBuild()
{
	return unitToBuild;
}

TilePosition BOInstruction::getBuildLocation()
{
	return buildLocation;
}

void BOInstruction::decrementNbUnits()
{
	nbUnitsToBuild--;
}

const char* BOInstruction::typeToStr()
{
	switch (type)
	{
		case NB_WORKERS:   
			return "NB_WORKERS";
			break;
		case BUILDING_COMPLETION:   
			return "BUILDING_COMPLETION";
			break;
		case RESOURCE_CAP: 
			return "RESOURCE_CAP";
			break;
		default :
			return "END_OF_BO";
			break;
	}
}

const char* BOInstruction::typeToStr(BOInstruction instruction)
{
	return instruction.typeToStr();
}

BOInstruction::InstructionType BOInstruction::strToType(const char* str)
{
	if (strcmp(str, "NB_WORKERS") == 0)
	{
		return NB_WORKERS;
	}
	else if (strcmp(str, "BUILDING_COMPLETION") == 0)
	{
		return BUILDING_COMPLETION;
	}
	else if (strcmp(str, "RESOURCE_CAP") == 0)
	{
		return RESOURCE_CAP;
	}
	else
	{
		return END_OF_BO;
	}
}
