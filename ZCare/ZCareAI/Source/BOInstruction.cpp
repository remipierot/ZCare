#include "BOInstruction.h"

using namespace BWAPI;

BOInstruction::BOInstruction()
{
	type = END_OF_BO;
	completed = true;
	baseIndex = 0;
	researchToMake = TechTypes::None;
	upgradeToMake = UpgradeTypes::None;
}

BOInstruction::BOInstruction(InstructionType newType)
{
	type = newType;
	completed = (newType == END_OF_BO) ? true : false;
	baseIndex = 0;
	researchToMake = TechTypes::None;
	upgradeToMake = UpgradeTypes::None;
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

void BOInstruction::setResearch(TechType newResearchToMake)
{
	researchToMake = newResearchToMake;
}

void BOInstruction::setUpgrade(UpgradeType newUpgradeToMake)
{
	upgradeToMake = newUpgradeToMake;
}

void BOInstruction::setBaseIndex(int baseLocationId)
{
	baseIndex = baseLocationId;
}

void BOInstruction::setNbUnitsOfType(int newNbUnitsOfType)
{
	nbUnitsOfType = newNbUnitsOfType;
}

BOInstruction::InstructionType BOInstruction::getType()
{
	return 
		type;
}

int BOInstruction::getSupplyCount()
{
	return 
		supplyCount;
}

int BOInstruction::getMineralCap()
{
	return 
		mineralCap;
}

int BOInstruction::getVespeneCap()
{
	return 
		vespeneCap;
}

int BOInstruction::getBuildingCompletionPercentage()
{
	return 
		buildingCompletionPercentage;
}

UnitType BOInstruction::getBuildingToTrack()
{
	return 
		buildingToTrack;
}

int BOInstruction::getNbUnitsToBuild()
{
	return 
		nbUnitsToBuild;
}

UnitType BOInstruction::getUnitToBuild()
{
	return 
		unitToBuild;
}

TilePosition BOInstruction::getBuildLocation()
{
	return 
		buildLocation;
}

TechType BOInstruction::getResearch()
{
	return 
		researchToMake;
}

UpgradeType BOInstruction::getUpgrade()
{
	return 
		upgradeToMake;
}

int BOInstruction::getBaseIndex()
{
	return 
		baseIndex;
}

int BOInstruction::getNbUnitsOfType()
{
	return
		nbUnitsOfType;
}

void BOInstruction::decrementNbUnits()
{
	nbUnitsToBuild--;
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
	return 
		completed;
}

bool BOInstruction::isResearch()
{
	return 
		researchToMake != TechTypes::None;
}

bool BOInstruction::isUpgrade()
{
	return 
		upgradeToMake != UpgradeTypes::None;
}

bool BOInstruction::isEvolution()
{
	return 
		unitToBuild != UnitTypes::None &&
		(unitToBuild == UnitTypes::Zerg_Lair ||
		unitToBuild == UnitTypes::Zerg_Hive ||
		unitToBuild == UnitTypes::Zerg_Sunken_Colony ||
		unitToBuild == UnitTypes::Zerg_Spore_Colony ||
		unitToBuild == UnitTypes::Zerg_Greater_Spire);
}

bool BOInstruction::isBuilding()
{
	return 
		unitToBuild != UnitTypes::None && 
		unitToBuild.isBuilding() &&
		!isEvolution();
}

bool BOInstruction::isUnit()
{
	return 
		unitToBuild != UnitTypes::None && 
		!isResearch() && 
		!isUpgrade() && 
		!isBuilding();
}

BOInstruction::InstructionType BOInstruction::strToType(const char* str)
{
	InstructionType type = END_OF_BO;

	if (strcmp(str, "SUPPLY_USED") == 0)
	{
		type = SUPPLY_USED;
	}
	else if (strcmp(str, "BUILDING_COMPLETION") == 0)
	{
		type = BUILDING_COMPLETION;
	}
	else if (strcmp(str, "RESOURCE_CAP") == 0)
	{
		type = RESOURCE_CAP;
	}
	else
	{
		type = END_OF_BO;
	}

	return
		type;
}
