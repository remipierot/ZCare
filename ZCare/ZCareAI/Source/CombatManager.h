#pragma once
#include <BWAPI.h>
#include "ToolBox.h"
#include "Squad.h"

class CombatManager
{
public:
	CombatManager();
	void update();
	void addSquad(Squad* squad);
	Squad* findSquad(int idSquad);
	int squadNumber();
	std::set<Squad*> getSquadList();
	

private:
	std::set<Squad*> squadList;
};

