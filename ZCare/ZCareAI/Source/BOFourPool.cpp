#include "BOFourPool.h"

using namespace BWAPI;

BOFourPool::BOFourPool()
{
	BOInstruction* tmp;

	tmp = new BOInstruction(BOInstruction::SUPPLY_USED);
	tmp->setSupplyCount(4);
	tmp->setActions(UnitTypes::Zerg_Spawning_Pool);
	addInstruction(tmp);

	tmp = new BOInstruction(BOInstruction::SUPPLY_USED);
	tmp->setSupplyCount(3);
	tmp->setActions(UnitTypes::Zerg_Drone);
	addInstruction(tmp);

	tmp = new BOInstruction(BOInstruction::SUPPLY_USED);
	tmp->setSupplyCount(4);
	tmp->setActions(UnitTypes::Zerg_Zergling, 3);
	addInstruction(tmp);

	tmp = new BOInstruction(BOInstruction::SUPPLY_USED);
	tmp->setSupplyCount(7);
	tmp->setActions(UnitTypes::Zerg_Zergling);
	addInstruction(tmp);

	tmp = new BOInstruction(BOInstruction::SUPPLY_USED);
	tmp->setSupplyCount(8);
	tmp->setActions(UnitTypes::Zerg_Zergling);
	addInstruction(tmp);
}
