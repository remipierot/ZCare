#pragma once
#include <BWAPI.h>

class ToolBox
{
	public:

		//Structure pour contenir un unite de gaz ou de mineraux avec un id de base
		struct Mineral
		{
			int idParent;
			BWAPI::Unit mineralUnit;

			Mineral(BWAPI::Unit mineralUnit)
			{
				idParent = -1;
				this->mineralUnit = mineralUnit;
			}
		};

		//Structure de base contenant la liste de mineral et de gaz
		struct Base
		{
				int idBase;
				std::set<ToolBox::Mineral> *mineralFields;
				std::set<ToolBox::Mineral> *gazFields;
				bool isExpansionInteressting;
				bool isStartingLocation;
				bool isEnnemyLocation;

				Base()
				{
					idBase = 0;
					mineralFields = new std::set<ToolBox::Mineral>();
					gazFields = new std::set<ToolBox::Mineral>();
					isExpansionInteressting = false;
					isStartingLocation = false;
					isEnnemyLocation = false;
				}
		};




		//Use a UnitType combined to its TilePosition to get the Position corresponding to its center
		static BWAPI::Position ConvertTilePosition(
			const BWAPI::TilePosition tilePosition,
			const BWAPI::UnitType unitType
		);

		//Check if the given unit is available (not building anything and no pending command on it)
		static bool IsWorkerAvailableToBuild(
			const BWAPI::Unit& unit
		);

		//Check if BWAPI already has a command pending to be executed for the specified unit
		static bool IsCommandPendingOnUnit(
			const BWAPI::Unit& unit
		);

		//Tell if the given TilePosition is a valid one (not invalid, unknown or not set)
		static bool IsTilePositionValid(
			const BWAPI::TilePosition tilePosition
		);

		//Tell if the given Position is a valid one (not invalid, unknown or not set)
		static bool IsPositionValid(
			const BWAPI::Position position
		);

<<<<<<< HEAD
		static bool IsInCircle(
			int posX, int posY, int radius, int pos2X, int pos2y, int radius2
			);
=======
		static const char RED_CHAR = '\x08';
		static const char GREEN_CHAR = '\x07';
		static const char WHITE_CHAR = '\x04';
>>>>>>> 6feea932c4d071a9ff45816d1f745295cc88fcab
};

