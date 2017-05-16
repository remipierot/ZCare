#include "Base.h"

using namespace std;
using namespace BWAPI;

Base::Base()
{
	idBase = 0;
	gazField = 0;
	isExpansionInteresting = false;
	isStartingLocation = false;
	isEnnemyLocation = false;
	isInvalidToGroundUnits = false;
	distanceToMainBase = 0;
	position = Positions::None;
	tilePosition = TilePositions::None;
	lastTimeChecked = 0;
}

Base::Base(Base* b)
{
	idBase = b->idBase;
	gazField = b->gazField;

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
	lastTimeChecked = b->lastTimeChecked;
}

void Base::computePosition()
{
	if (position == Positions::None)
	{
		Position p(0, 0);
		Position tmp(0, 0);

		int resourceCount = 0;

		if (gazField != 0)
		{
			tmp = gazField->resourceUnit->getPosition();
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

void Base::computeTilePosition()
{
	if (tilePosition == TilePositions::None)
	{
		TilePosition tp(0, 0);
		TilePosition tmp(0, 0);

		int resourceCount = 0;

		if (gazField != 0)
		{
			tmp = gazField->resourceUnit->getTilePosition();
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

float Base::getDistanceToUnit(Unit u)
{
	return (float)position.getDistance(u->getPosition());
}

void Base::setDistanceToMainBase(Unit u)
{
	distanceToMainBase = (int)getDistanceToUnit(u);
}

void Base::printBase()
{
	char textColor = (isEnnemyLocation) ? ToolBox::BRIGHT_RED_CHAR :
						(isStartingLocation) ? ToolBox::YELLOW_CHAR :
						(isExpansionInteresting) ? ToolBox::BRIGHT_GREEN_CHAR :
						ToolBox::WHITE_CHAR;

	Broodwar->sendText(
		"%c %d - [%d, %d] - %d",
		textColor,
		idBase,
		position.x,
		position.y,
		distanceToMainBase
	);
}

bool Base::hasToBeChecked()
{
	return (Broodwar->getFrameCount() - lastTimeChecked) > FRAMES_UNTIL_CHECK_BASE;
}
