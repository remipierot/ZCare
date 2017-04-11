#include "ToolBox.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

//Use a UnitType combined to its TilePosition to get the Position corresponding to its center
Position ToolBox::ConvertTilePosition(const TilePosition tilePosition, const UnitType unitType)
{
	return
		Position(
			Position(tilePosition) + 
			Position(
				(unitType.tileWidth() * TILEPOSITION_SCALE) / 2,
				(unitType.tileHeight() * TILEPOSITION_SCALE) / 2
			)
		);
}

//Tell if the given unit is available (not building anything and no pending command on it)
bool ToolBox::IsWorkerAvailableToBuild(const Unit& unit)
{
	return
		!unit->isConstructing() && 
		!IsCommandPendingOnUnit(unit);
}

//Tell if BWAPI already has a command pending to be executed for the specified unit
bool ToolBox::IsCommandPendingOnUnit(const Unit& unit)
{
	return
		!(unit->getLastCommand().getType() == UnitCommandTypes::None || 
			Broodwar->getFrameCount() >= (
				unit->getLastCommandFrame() + (
					Broodwar->getLatencyFrames() > 2
					? Broodwar->getLatencyFrames() - (unit->getLastCommandFrame() % 2)
					: Broodwar->getLatencyFrames()
				)
			)
		);
}

//Tell if the given TilePosition is a valid one (not invalid, unknown or not set)
bool ToolBox::IsTilePositionValid(const TilePosition tilePosition)
{
	return 
		tilePosition != TilePositions::Invalid && 
		tilePosition != TilePositions::Unknown && 
		tilePosition != TilePositions::None;
}

//Tell if the given Position is a valid one (not invalid, unknown or not set)
bool ToolBox::IsPositionValid(const Position position)
{
	return
		position != Positions::Invalid &&
		position != Positions::Unknown &&
		position != Positions::None;
}


bool ToolBox::IsInCircle(int posX, int posY, int radius, int posX2, int posY2, int radius2)
{
	return
		pow((posX2 - posX), 2) + pow((posY - posY2),2) <= pow((radius + radius2),2);
}
