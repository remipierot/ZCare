#include "BOInstruction.h"

using namespace BWAPI;

BOInstruction::BOInstruction()
{
	type = END_OF_BO;
}

BOInstruction::BOInstruction(InstructionType newType)
{
	type = newType;
	completed = (newType == END_OF_BO) ? true : false;
}

void BOInstruction::setSupplyCount(int newSupplyCount)
{
	supplyCount = newSupplyCount;
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

int BOInstruction::getSupplyCount()
{
	return supplyCount;
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
		case SUPPLY_USED:   
			return "SUPPLY_USED";
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
	if (strcmp(str, "SUPPLY_USED") == 0)
	{
		return SUPPLY_USED;
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

void BOInstruction::reset()
{
	completed = false;
}

void BOInstruction::complete()
{
	completed = true;
}

bool BOInstruction::isCompleted()
{
	return completed;
}

void BOInstruction::setNbUnitsOfType(int newNbUnitsOfType)
{
	nbUnitsOfType = newNbUnitsOfType;
}

int BOInstruction::getNbUnitsOfType()
{
	return nbUnitsOfType;
}
