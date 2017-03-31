#pragma once
#include "BuildOrder.h"
#include <windows.h>
#include "tinyxml2.h"

class BOParser
{
public:
	BOParser(BuildOrder *buildOrder);
	~BOParser();

private:
	BuildOrder *bo;

};

