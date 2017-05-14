#pragma once

#include "ToolBox.h"
#include "Resource.h"

class Base
{
public:
	Base();
	Base(Base* b);

	int idBase;
	std::set<Resource*> mineralFields;
	Resource* gazField;
	bool isExpansionInteresting;
	bool isStartingLocation;
	bool isEnnemyLocation;
	int distanceToMainBase;
	BWAPI::Position position;
	BWAPI::TilePosition tilePosition;

	void computePosition();
	void computeTilePosition();
	float getDistanceToUnit(BWAPI::Unit u);
	void setDistanceToMainBase(BWAPI::Unit u);
	void printBase();
};

