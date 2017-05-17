#pragma once

#include "ToolBox.h"

class Resource
{
	public:
		int idParent;				// ID to identify the parent base
		BWAPI::Unit resourceUnit;	// Unit linked to the resource

		Resource();

		Resource(
			BWAPI::Unit resourceUnit
		);
};

