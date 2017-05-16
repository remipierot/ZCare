#include "Base.h"

using namespace std;
using namespace BWAPI;

Base::Base()
{
	idBase = 0;
	geyser = 0;
	isExpansionInteresting = false;
	isStartingLocation = false;
	isEnnemyLocation = false;
	isInvalidToGroundUnits = false;
	distanceToMainBase = 0;
	position = Positions::None;
	tilePosition = TilePositions::None;
	lastFrameChecked = 0;
}

Base::Base(Base* b)
{
	idBase = b->idBase;
	geyser = b->geyser;

	for (Resource* mineral : b->mineralFields)
	{
		mineralFields.insert(mineral);
	}

	isExpansionInteresting = b->isExpansionInteresting;
	isStartingLocation = b->isStartingLocation;
	isEnnemyLocation = b->isEnnemyLocation;
	isInvalidToGroundUnits = b->isInvalidToGroundUnits;
	distanceToMainBase = b->distanceToMainBase;
	position = b->position;
	tilePosition = b->tilePosition;
	lastFrameChecked = b->lastFrameChecked;
}

// Compute the position based on each resource (minerals + gas)
void Base::computePosition()
{
	if (position == Positions::None)
	{
		Position p(0, 0);
		Position tmp(0, 0);

		int resourceCount = 0;

		if (geyser != 0)
		{
			tmp = geyser->resourceUnit->getPosition();
			p.x += tmp.x;
			p.y += tmp.y;
			resourceCount++;
		}

		for (Resource* mineralField : mineralFields)
		{
			tmp = mineralField->resourceUnit->getPosition();
			p.x += tmp.x;
			p.y += tmp.y;
			resourceCount++;
		}

		p.x /= resourceCount;
		p.y /= resourceCount;

		position = p;
	}
}

// Compute the tilePosition based on each resource (minerals + gas)
void Base::computeTilePosition()
{
	if (tilePosition == TilePositions::None)
	{
		TilePosition tp(0, 0);
		TilePosition tmp(0, 0);

		int resourceCount = 0;

		if (geyser != 0)
		{
			tmp = geyser->resourceUnit->getTilePosition();
			tp.x += tmp.x;
			tp.y += tmp.y;
			resourceCount++;
		}

		for (Resource* mineralField : mineralFields)
		{
			tmp = mineralField->resourceUnit->getTilePosition();
			tp.x += tmp.x;
			tp.y += tmp.y;
			resourceCount++;
		}

		tp.x /= resourceCount;
		tp.y /= resourceCount;

		tilePosition = tp;
	}
}

// Insert a new mineral inside mineralFields
void Base::insertMineral(Resource* newMineral)
{
	mineralFields.insert(newMineral);
}

// Store the given geyser as the one linked to the base
void Base::setGeyser(Resource* newGeyser)
{
	geyser = newGeyser;
}

// Give the position of the base
Position Base::getPosition()
{
	return position;
}

// Give the tilePosition of the base
TilePosition Base::getTilePosition()
{
	return tilePosition;
}

// Give the list of mineralFields
set<Resource*> Base::getMineralFields()
{
	return mineralFields;
}

// Give the geyser
Resource* Base::getGeyser()
{
	return geyser;
}

// True if the base has not been checked for a while (exact number set in ToolBox)
bool Base::hasToBeChecked()
{
	return (Broodwar->getFrameCount() - lastFrameChecked) > ToolBox::FRAMES_UNTIL_CHECK_BASE;
}
