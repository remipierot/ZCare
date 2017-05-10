#pragma once

#include "ToolBox.h"
#include "Resource.h"

class Base
{
public:
	Base();

	int idBase;
	std::set<Resource*> mineralFields;
	Resource* gazFields;
	bool isExpansionInteresting;
	bool isStartingLocation;
	bool isEnnemyLocation;
	BWAPI::Position baseLocation;

};

