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

//********** INSTRUCTION MANAGEMENT **********//
// Get the type
BOInstruction::InstructionType BOInstruction::getType()
{
	return
		type;
}

// Remove 1 unit to the nbUnitsToBuild count
void BOInstruction::decrementNbUnits()
{
	nbUnitsToBuild--;
}

// Set the instruction as not completed
void BOInstruction::reset()
{
	completed = false;
}

// Set the instruction as completed
void BOInstruction::complete()
{
	completed = true;
}

// Tell if the instruction is completed
bool BOInstruction::isCompleted()
{
	return
		completed;
}

// Tell if the instruction is about making a research
bool BOInstruction::isResearch()
{
	return
		researchToMake != TechTypes::None;
}

// Tell if the instruction is about making an upgrade
bool BOInstruction::isUpgrade()
{
	return
		upgradeToMake != UpgradeTypes::None;
}

// Tell if the instruction is about evolving a building
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

// Tell if the instruction is about making a building
bool BOInstruction::isBuilding()
{
	return
		unitToBuild != UnitTypes::None &&
		unitToBuild.isBuilding() &&
		!isEvolution();
}

// Tell if the instruction is about making a unit
bool BOInstruction::isUnit()
{
	return
		unitToBuild != UnitTypes::None &&
		!isResearch() &&
		!isUpgrade() &&
		!isBuilding();
}

// Convert the given string to an instruction type
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
//********** END INSTRUCTION MANAGEMENT **********//

//********** REQUIREMENTS **********//
// Store the given supply count as the requirement
void BOInstruction::setSupplyCount(int newSupplyCount)
{
	supplyCount = newSupplyCount;
}

// Store the given mineral cap as the requirement
void BOInstruction::setMineralCap(int newMineralCap)
{
	mineralCap = newMineralCap;
}

// Store the given gas cap as the requirement
void BOInstruction::setVespeneCap(int newVespeneCap)
{
	vespeneCap = newVespeneCap;
}

// Store the given percentage and building as the requirement
void BOInstruction::setBuildingCompletionInfos(int newPercentage, UnitType newBuildingToTrack)
{
	buildingCompletionPercentage = newPercentage;
	buildingToTrack = newBuildingToTrack;
}

// Get the supply count requirement
int BOInstruction::getSupplyCount()
{
	return
		supplyCount;
}

// Get the mineral count requirement
int BOInstruction::getMineralCap()
{
	return
		mineralCap;
}

// Get the gas count requirement
int BOInstruction::getVespeneCap()
{
	return
		vespeneCap;
}

// Get the building percentage requirement
int BOInstruction::getBuildingCompletionPercentage()
{
	return
		buildingCompletionPercentage;
}

// Get the building that needs to be tracked
UnitType BOInstruction::getBuildingToTrack()
{
	return
		buildingToTrack;
}
//********** END REQUIREMENTS **********//

//********** ACTIONS **********//
// Store the given unit type and its count as the action
void BOInstruction::setUnit(UnitType newUnitToBuild, int newNbUnitsToBuild)
{
	unitToBuild = newUnitToBuild;
	nbUnitsToBuild = newNbUnitsToBuild;
}

// Store the given research as the action
void BOInstruction::setResearch(TechType newResearchToMake)
{
	researchToMake = newResearchToMake;
}

// Store the given upgrade as the action
void BOInstruction::setUpgrade(UpgradeType newUpgradeToMake)
{
	upgradeToMake = newUpgradeToMake;
}

// Store the given index as the base location where the instruction will be executed
void BOInstruction::setBaseIndex(int baseLocationId)
{
	baseIndex = baseLocationId;
}

// Get the number of units to build
int BOInstruction::getNbUnitsToBuild()
{
	return
		nbUnitsToBuild;
}

// Get the unit type to build
UnitType BOInstruction::getUnitToBuild()
{
	return
		unitToBuild;
}

// Get the research to make
TechType BOInstruction::getResearch()
{
	return
		researchToMake;
}

// Get the upgrade to make
UpgradeType BOInstruction::getUpgrade()
{
	return
		upgradeToMake;
}

// Get the base location index
int BOInstruction::getBaseIndex()
{
	return
		baseIndex;
}
//********** END  ACTIONS **********//
