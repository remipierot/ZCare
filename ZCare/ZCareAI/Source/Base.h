#pragma once

#include "ToolBox.h"
#include "Resource.h"

class Base
{
	public:
		int idBase;						// ID to identify the base
		bool isExpansionInteresting;	// True if the base has minerals AND gas
		bool isStartingLocation;		// True if it is one of the starting locations given by BWAPI
		bool isEnnemyLocation;			// True if it can be an enemy starting location
		bool isInvalidToGroundUnits;	// True if it can't be reached by grounded units
		int distanceToMainBase;			// Distance between the base and our starting location
		int lastFrameChecked;			// Last frame when the base was visible

		Base();

		Base(
			Base* b
		);

		// Compute the position based on each resource (minerals + gas)
		void computePosition();

		// Compute the tilePosition based on each resource (minerals + gas)
		void computeTilePosition();

		// Insert a new mineral inside mineralFields
		void insertMineral(
			Resource* newMineral
		);

		// Store the given geyser as the one linked to the base
		void setGeyser(
			Resource* newGeyser
		);

		// Give the position of the base
		BWAPI::Position getPosition();

		// Give the tilePosition of the base
		BWAPI::TilePosition getTilePosition();

		// Give the list of mineralFields
		std::set<Resource*> getMineralFields();

		// Give the geyser
		Resource* getGeyser();

		// True if the base has not been checked for a while (exact number set in ToolBox)
		bool hasToBeChecked();

	private:
		std::set<Resource*> mineralFields;	// Every mineral field linked to the base
		Resource* geyser;					// Geyser linked to the base, can be null
		BWAPI::Position position;			// Position of the base according to its resources
		BWAPI::TilePosition tilePosition;	// TilePosition of the base according to its resources
};

