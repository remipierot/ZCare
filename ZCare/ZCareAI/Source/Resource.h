#pragma once

#include "ToolBox.h"

class Resource
{
public:
	int idParent;
	BWAPI::Unit resourceUnit;

	Resource();
	Resource(BWAPI::Unit resourceUnit);
};

