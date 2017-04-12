#include "Resource.h"

using namespace BWAPI;

Resource::Resource()
{
	idParent = -1;
	this->resourceUnit = NULL;
}

Resource::Resource(Unit resourceUnit)
{
	idParent = -1;
	this->resourceUnit = resourceUnit;
}
