#include "ToolBox.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

//Use a UnitType combined to its TilePosition to get the Position corresponding to its center
Position ToolBox::ConvertTilePosition(const TilePosition tilePosition, const UnitType unitType)
{
	return
		Position(
			Position(tilePosition) + 
			Position(
				(unitType.tileWidth() * TILEPOSITION_SCALE) / 2,
				(unitType.tileHeight() * TILEPOSITION_SCALE) / 2
			)
		);
}

//Tell if the given unit is available (not building anything and no pending command on it)
bool ToolBox::IsWorkerAvailableToBuild(const Unit& unit)
{
	return
		!unit->isConstructing() && 
		!IsCommandPendingOnUnit(unit);
}

//Tell if BWAPI already has a command pending to be executed for the specified unit
bool ToolBox::IsCommandPendingOnUnit(const Unit& unit)
{
	return
		!(unit->getLastCommand().getType() == UnitCommandTypes::None || 
			Broodwar->getFrameCount() >= (
				unit->getLastCommandFrame() + (
					Broodwar->getLatencyFrames() > 2
					? Broodwar->getLatencyFrames() - (unit->getLastCommandFrame() % 2)
					: Broodwar->getLatencyFrames()
				)
			)
		);
}

//Tell if the given TilePosition is a valid one (not invalid, unknown or not set)
bool ToolBox::IsTilePositionValid(const TilePosition tilePosition)
{
	return 
		tilePosition != TilePositions::Invalid && 
		tilePosition != TilePositions::Unknown && 
		tilePosition != TilePositions::None;
}

//Tell if the given Position is a valid one (not invalid, unknown or not set)
bool ToolBox::IsPositionValid(const Position position)
{
	return
		position != Positions::Invalid &&
		position != Positions::Unknown &&
		position != Positions::None;
}


bool ToolBox::IsInCircle(int posX, int posY, int radius, int posX2, int posY2, int radius2)
{
	return
		pow((posX2 - posX), 2) + pow((posY - posY2),2) <= pow((radius + radius2),2);
}

bool ToolBox::IsInCircle(Position p1, int r1, Position p2, int r2)
{
	return
		IsInCircle(p1.x, p1.y, r1, p2.x, p2.y, r2);
}

bool ToolBox::areUnitsLinkedByEvolution(const UnitType unit1, const UnitType unit2)
{
	bool linkedByEvolution = false;

	//Spire evolution
	linkedByEvolution |= (unit1 == UnitTypes::Zerg_Greater_Spire && unit2 == UnitTypes::Zerg_Spire);
	linkedByEvolution |= (unit2 == UnitTypes::Zerg_Greater_Spire && unit1 == UnitTypes::Zerg_Spire);

	//Hatchery evolution
	linkedByEvolution |= (unit1 == UnitTypes::Zerg_Lair && unit2 == UnitTypes::Zerg_Hatchery);
	linkedByEvolution |= (unit2 == UnitTypes::Zerg_Lair && unit1 == UnitTypes::Zerg_Hatchery);
	linkedByEvolution |= (unit1 == UnitTypes::Zerg_Hive && unit2 == UnitTypes::Zerg_Hatchery);
	linkedByEvolution |= (unit2 == UnitTypes::Zerg_Hive && unit1 == UnitTypes::Zerg_Hatchery);
	linkedByEvolution |= (unit1 == UnitTypes::Zerg_Hive && unit2 == UnitTypes::Zerg_Lair);
	linkedByEvolution |= (unit2 == UnitTypes::Zerg_Hive && unit1 == UnitTypes::Zerg_Lair);

	//Creep Colony evolution
	linkedByEvolution |= (unit1 == UnitTypes::Zerg_Sunken_Colony && unit2 == UnitTypes::Zerg_Creep_Colony);
	linkedByEvolution |= (unit2 == UnitTypes::Zerg_Sunken_Colony && unit1 == UnitTypes::Zerg_Creep_Colony);
	linkedByEvolution |= (unit1 == UnitTypes::Zerg_Spore_Colony && unit2 == UnitTypes::Zerg_Creep_Colony);
	linkedByEvolution |= (unit2 == UnitTypes::Zerg_Spore_Colony && unit1 == UnitTypes::Zerg_Creep_Colony);

	//Same unit type
	linkedByEvolution |= (unit1 == unit2);

	return linkedByEvolution;
}

UnitType ToolBox::getUnitAbleToResearch(TechType researchToMake)
{
	UnitType unit = UnitTypes::None;

	bool hatchery = false;
	bool lair = false;
	bool hive = false;
	bool hydraliskDen = false;
	bool queensNest = false;
	bool defilerMound = false;

	hatchery |= researchToMake == TechTypes::Burrowing;
	lair |= hatchery;
	hive |= hatchery;

	hydraliskDen |= researchToMake == TechTypes::Lurker_Aspect;

	queensNest |= researchToMake == TechTypes::Spawn_Broodlings;
	queensNest |= researchToMake == TechTypes::Ensnare;

	defilerMound |= researchToMake == TechTypes::Plague;
	defilerMound |= researchToMake == TechTypes::Consume;

	unit = 
		(hatchery || lair || hive)	? UnitTypes::Zerg_Hatchery		:
		(hydraliskDen)				? UnitTypes::Zerg_Hydralisk_Den :
		(queensNest)				? UnitTypes::Zerg_Queens_Nest	:
		(defilerMound)				? UnitTypes::Zerg_Defiler_Mound :
		UnitTypes::None;

	return unit;
}

UnitType ToolBox::getUnitAbleToUpgrade(UpgradeType upgradeToMake)
{
	UnitType unit = UnitTypes::None;

	bool lair = false;
	bool hive = false;
	bool evolutionChamber = false;
	bool spawningPool = false;
	bool hydraliskDen = false;
	bool queensNest = false;
	bool spire = false;
	bool greaterSpire = false;
	bool defilerMound = false;
	bool ultraliskCavern = false;

	lair |= upgradeToMake == UpgradeTypes::Ventral_Sacs;
	lair |= upgradeToMake == UpgradeTypes::Antennae;
	lair |= upgradeToMake == UpgradeTypes::Pneumatized_Carapace;
	hive |= lair;

	evolutionChamber |= upgradeToMake == UpgradeTypes::Zerg_Melee_Attacks;
	evolutionChamber |= upgradeToMake == UpgradeTypes::Zerg_Missile_Attacks;
	evolutionChamber |= upgradeToMake == UpgradeTypes::Zerg_Carapace;

	spawningPool |= upgradeToMake == UpgradeTypes::Metabolic_Boost;
	spawningPool |= upgradeToMake == UpgradeTypes::Adrenal_Glands;

	hydraliskDen |= upgradeToMake == UpgradeTypes::Muscular_Augments;
	hydraliskDen |= upgradeToMake == UpgradeTypes::Grooved_Spines;

	queensNest |= upgradeToMake == UpgradeTypes::Gamete_Meiosis;

	spire |= upgradeToMake == UpgradeTypes::Zerg_Flyer_Attacks;
	spire |= upgradeToMake == UpgradeTypes::Zerg_Flyer_Carapace;
	greaterSpire |= spire;

	defilerMound |= upgradeToMake == UpgradeTypes::Metasynaptic_Node;

	ultraliskCavern |= upgradeToMake == UpgradeTypes::Anabolic_Synthesis;
	ultraliskCavern |= upgradeToMake == UpgradeTypes::Chitinous_Plating;

	unit =
		(lair || hive)			? UnitTypes::Zerg_Lair				:
		(evolutionChamber)		? UnitTypes::Zerg_Evolution_Chamber :
		(spawningPool)			? UnitTypes::Zerg_Spawning_Pool		:
		(hydraliskDen)			? UnitTypes::Zerg_Hydralisk_Den		:
		(queensNest)			? UnitTypes::Zerg_Queens_Nest		:
		(spire || greaterSpire) ? UnitTypes::Zerg_Spire				:
		(defilerMound)			? UnitTypes::Zerg_Defiler_Mound		:
		(ultraliskCavern)		? UnitTypes::Zerg_Ultralisk_Cavern	:
		UnitTypes::None;

	return unit;
}
