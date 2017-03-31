#pragma once
#include "BuildOrder.h"
#include <windows.h>
#include "tinyxml2.h"

class BOParser
{
public:
	BOParser();
	BOParser(BuildOrder *buildOrder);

private:
	BuildOrder *bo;
};

