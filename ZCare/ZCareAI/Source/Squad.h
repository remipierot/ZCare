#pragma once
#include <set>
#include "BWAPI.h"
#include "ToolBox.h"

class Squad
{
public:
	Squad(int squad);
	Squad(std::set<const BWAPI::Unit> terrainUnit, std::set<const BWAPI::Unit> aerialUnit);
	~Squad();

	void moveToLocation(BWAPI::PositionOrUnit target);
	void attackOrMove(BWAPI::PositionOrUnit target);
	bool insertUnit(BWAPI::Unit unit);
	
	int numberUnit();
	int getIdSquad();

	std::set<const BWAPI::Unit> getTerrainUnit();
	std::set<const BWAPI::Unit> getAerialUnit();


private :
	int idSquad;
	int numberUnitMax;
	int currentNumberUnit;
	std::set<const BWAPI::Unit> terrainUnit;
	std::set<const BWAPI::Unit> aerialUnit;

};

