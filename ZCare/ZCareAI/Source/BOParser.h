#pragma once
#include "BuildOrder.h"
#include <windows.h>
#include "tinyxml2.h"

class BOParser
{
	public:
		// Return the BO loaded with the given path
		static BuildOrder loadBO(
			std::string fileName
		);
};

