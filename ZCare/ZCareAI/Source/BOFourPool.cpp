#include "BOFourPool.h"

using namespace BWAPI;

BOFourPool::BOFourPool()
{
	BOInstruction* tmp;

	tmp = new BOInstruction(BOInstruction::NB_WORKERS);
	tmp->setWorkerCount(4);
	tmp->setActions(UnitTypes::Zerg_Spawning_Pool);
	addInstruction(tmp);

	tmp = new BOInstruction(BOInstruction::NB_WORKERS);
	tmp->setWorkerCount(3);
	tmp->setActions(UnitTypes::Zerg_Drone);
	addInstruction(tmp);

	tmp = new BOInstruction(BOInstruction::NB_WORKERS);
	tmp->setWorkerCount(4);
	tmp->setActions(UnitTypes::Zerg_Zergling, 3);
	addInstruction(tmp);

	tmp = new BOInstruction(BOInstruction::NB_WORKERS);
	tmp->setWorkerCount(7);
	tmp->setActions(UnitTypes::Zerg_Zergling);
	addInstruction(tmp);

	tmp = new BOInstruction(BOInstruction::NB_WORKERS);
	tmp->setWorkerCount(8);
	tmp->setActions(UnitTypes::Zerg_Zergling);
	addInstruction(tmp);
}
