#pragma once
#include <BWAPI.h>
#include "ToolBox.h"
#include "Squad.h"
#include "Base.h"

class CombatManager
{
public:
	CombatManager();
	void update();
	void addSquad(Squad* squad);
	Squad* findSquad(int idSquad);
	int squadNumber();
	std::set<Squad*> getSquadList();
	void modeDefense(Squad* squad);
	void modeAttack(Squad* squad);

	void setDiscoveredUnits(std::set<const BWAPI::Unit> *unit);

	void setAllBaseLocations(std::set<Base*> newAllBaseLocations);

	void traitementAttack(std::set<BWAPI::Unit> *erase, std::set<const BWAPI::Unit> *unitType, Squad *squad, bool isGrounded);
	

private:
	std::set<Squad*> squadList;
	std::set<const BWAPI::Unit> *unitDiscover;
	std::set<Base*> allBaseLocations;
	std::set<BWAPI::Unit> *unitToAttack;
};

