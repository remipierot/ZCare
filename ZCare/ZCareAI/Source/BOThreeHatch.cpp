#include "BOThreeHatch.h"

using namespace BWAPI;

BOThreeHatch::BOThreeHatch()
{
	BOInstruction* tmp;

	tmp = new BOInstruction(BOInstruction::SUPPLY_USED);
	tmp->setSupplyCount(9);
	tmp->setActions(UnitTypes::Zerg_Overlord);
	addInstruction(tmp);

	tmp = new BOInstruction(BOInstruction::SUPPLY_USED);
	tmp->setSupplyCount(12);
	tmp->setActions(UnitTypes::Zerg_Hatchery);
	addInstruction(tmp);

	tmp = new BOInstruction(BOInstruction::SUPPLY_USED);
	tmp->setSupplyCount(11);
	tmp->setActions(UnitTypes::Zerg_Spawning_Pool);
	addInstruction(tmp);
}
