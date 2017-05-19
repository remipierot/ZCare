#include "ToolBox.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

// Use a UnitType combined to its TilePosition to get the Position corresponding to its center
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

// Tell if the given TilePosition is a valid one (not invalid, unknown or not set)
bool ToolBox::IsTilePositionValid(const TilePosition tilePosition)
{
	return 
		tilePosition != TilePositions::Invalid && 
		tilePosition != TilePositions::Unknown && 
		tilePosition != TilePositions::None;
}

// Tell if the two wanted circles overlap
bool ToolBox::IsInCircle(int x1, int y1, int r1, int x2, int y2, int r2)
{
	return
		pow((x1 - x2), 2) + pow((y1 - y2),2) <= pow((r1 + r2),2);
}

// Tell if the two wanted circles overlap
bool ToolBox::IsInCircle(Position p1, int r1, Position p2, int r2)
{
	return
		IsInCircle(p1.x, p1.y, r1, p2.x, p2.y, r2);
}

// Tell if the one of the units is an evolution of the other one
bool ToolBox::areUnitsLinkedByEvolution(const UnitType unit1, const UnitType unit2)
{
	bool linkedByEvolution = false;

	// Spire evolution
	linkedByEvolution |= (unit1 == UnitTypes::Zerg_Greater_Spire && unit2 == UnitTypes::Zerg_Spire);
	linkedByEvolution |= (unit2 == UnitTypes::Zerg_Greater_Spire && unit1 == UnitTypes::Zerg_Spire);

	// Hatchery evolution
	linkedByEvolution |= (unit1 == UnitTypes::Zerg_Lair && unit2 == UnitTypes::Zerg_Hatchery);
	linkedByEvolution |= (unit2 == UnitTypes::Zerg_Lair && unit1 == UnitTypes::Zerg_Hatchery);
	linkedByEvolution |= (unit1 == UnitTypes::Zerg_Hive && unit2 == UnitTypes::Zerg_Hatchery);
	linkedByEvolution |= (unit2 == UnitTypes::Zerg_Hive && unit1 == UnitTypes::Zerg_Hatchery);
	linkedByEvolution |= (unit1 == UnitTypes::Zerg_Hive && unit2 == UnitTypes::Zerg_Lair);
	linkedByEvolution |= (unit2 == UnitTypes::Zerg_Hive && unit1 == UnitTypes::Zerg_Lair);

	// Creep Colony evolution
	linkedByEvolution |= (unit1 == UnitTypes::Zerg_Sunken_Colony && unit2 == UnitTypes::Zerg_Creep_Colony);
	linkedByEvolution |= (unit2 == UnitTypes::Zerg_Sunken_Colony && unit1 == UnitTypes::Zerg_Creep_Colony);
	linkedByEvolution |= (unit1 == UnitTypes::Zerg_Spore_Colony && unit2 == UnitTypes::Zerg_Creep_Colony);
	linkedByEvolution |= (unit2 == UnitTypes::Zerg_Spore_Colony && unit1 == UnitTypes::Zerg_Creep_Colony);

	// Same unit type
	linkedByEvolution |= (unit1 == unit2);

	return 
		linkedByEvolution;
}

// Return the UnitType able to make the given research
UnitType ToolBox::getUnitAbleToResearch(TechType researchToMake)
{
	UnitType unit = UnitTypes::None;

	bool hatchery = false;
	bool lair = false;
	bool hive = false;
	bool hydraliskDen = false;
	bool queensNest = false;
	bool defilerMound = false;

	// Hatchery / Lair / Hive research
	hatchery |= researchToMake == TechTypes::Burrowing;
	lair |= hatchery;
	hive |= hatchery;

	// Hydralisk research
	hydraliskDen |= researchToMake == TechTypes::Lurker_Aspect;

	// Queen research
	queensNest |= researchToMake == TechTypes::Spawn_Broodlings;
	queensNest |= researchToMake == TechTypes::Ensnare;

	// Defiler research
	defilerMound |= researchToMake == TechTypes::Plague;
	defilerMound |= researchToMake == TechTypes::Consume;

	unit = 
		(hatchery || lair || hive)	? UnitTypes::Zerg_Hatchery		:
		(hydraliskDen)				? UnitTypes::Zerg_Hydralisk_Den :
		(queensNest)				? UnitTypes::Zerg_Queens_Nest	:
		(defilerMound)				? UnitTypes::Zerg_Defiler_Mound :
		UnitTypes::None;

	return 
		unit;
}

// Return the UnitType able to make the given upgrade
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

	// Lair / Hive upgrade
	lair |= upgradeToMake == UpgradeTypes::Ventral_Sacs;
	lair |= upgradeToMake == UpgradeTypes::Antennae;
	lair |= upgradeToMake == UpgradeTypes::Pneumatized_Carapace;
	hive |= lair;

	// Evolution Chamber upgrade
	evolutionChamber |= upgradeToMake == UpgradeTypes::Zerg_Melee_Attacks;
	evolutionChamber |= upgradeToMake == UpgradeTypes::Zerg_Missile_Attacks;
	evolutionChamber |= upgradeToMake == UpgradeTypes::Zerg_Carapace;

	// Zergling upgrade
	spawningPool |= upgradeToMake == UpgradeTypes::Metabolic_Boost;
	spawningPool |= upgradeToMake == UpgradeTypes::Adrenal_Glands;

	// Hydralisk upgrade
	hydraliskDen |= upgradeToMake == UpgradeTypes::Muscular_Augments;
	hydraliskDen |= upgradeToMake == UpgradeTypes::Grooved_Spines;

	// Queen upgrade
	queensNest |= upgradeToMake == UpgradeTypes::Gamete_Meiosis;

	// Spire / Greater Spire upgrade
	spire |= upgradeToMake == UpgradeTypes::Zerg_Flyer_Attacks;
	spire |= upgradeToMake == UpgradeTypes::Zerg_Flyer_Carapace;
	greaterSpire |= spire;

	// Defiler upgrade
	defilerMound |= upgradeToMake == UpgradeTypes::Metasynaptic_Node;

	// Ultralisk upgrade
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

	return 
		unit;
}

// Return the previous evolution UnitType of the given UnitType
UnitType ToolBox::getPreviousEvolution(UnitType currentEvolution)
{
	UnitType unit = UnitTypes::None;

	if (currentEvolution == UnitTypes::Zerg_Lair)
		unit = UnitTypes::Zerg_Hatchery;
	else if (currentEvolution == UnitTypes::Zerg_Hive)
		unit = UnitTypes::Zerg_Lair;
	else if (currentEvolution == UnitTypes::Zerg_Sunken_Colony)
		unit = UnitTypes::Zerg_Creep_Colony;
	else if (currentEvolution == UnitTypes::Zerg_Spore_Colony)
		unit = UnitTypes::Zerg_Creep_Colony;
	else if (currentEvolution == UnitTypes::Zerg_Greater_Spire)
		unit = UnitTypes::Zerg_Spire;

	return 
		unit;
}

// Tell if the given unit is ours or not
bool ToolBox::isUnitOurs(Unit unit)
{
	bool isOurUnit = false;

	for (Unit ourUnit : Broodwar->self()->getUnits())
	{
		if (ourUnit == unit)
		{
			isOurUnit = true;
			break;
		}
	}

	return isOurUnit;
}
