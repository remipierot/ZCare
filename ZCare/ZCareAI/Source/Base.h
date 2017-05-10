#pragma once

#include "ToolBox.h"
#include "Resource.h"

class Base
{
public:
	int idBase;
	std::set<Resource*> mineralFields;
	std::set<Resource*> gazFields;
	bool isExpansionInteresting;
	bool isStartingLocation;
	bool isEnnemyLocation;

	Base();
};

