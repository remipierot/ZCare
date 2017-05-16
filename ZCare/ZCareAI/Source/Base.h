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
	int lastTimeChecked;

	void computePosition();
	void computeTilePosition();
	float getDistanceToUnit(BWAPI::Unit u);
	void setDistanceToMainBase(BWAPI::Unit u);
	void printBase();
	bool hasToBeChecked();

	static const int BASE_FPS = 15;
	static const int SEC_UNTIL_CHECK_BASE = 30;
	static const int FRAMES_UNTIL_CHECK_BASE = BASE_FPS * SEC_UNTIL_CHECK_BASE;
};

