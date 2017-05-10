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

private:
	std::set<Squad*> squadList;
};

