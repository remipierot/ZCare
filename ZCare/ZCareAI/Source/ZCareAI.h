#pragma once
#include <BWAPI.h>
#include "GameManager.h"

// Remember not to use "Broodwar" in any global class constructor!
class ZCareAI : public BWAPI::AIModule
{
	public:
		// Virtual functions for callbacks, leave these as they are.
		virtual void onStart();

		virtual void onFrame();

		virtual void onSendText(
			std::string text
		);

		virtual void onReceiveText(
			BWAPI::Player player, 
			std::string text
		);

		virtual void onUnitEvade(
			BWAPI::Unit unit
		);

		virtual void onUnitShow(
			BWAPI::Unit unit
		);

		virtual void onUnitComplete(
			BWAPI::Unit unit
		);

		virtual void onEnd(
			bool isWinner
		);

		virtual void onPlayerLeft(
			BWAPI::Player player
		);

		virtual void onNukeDetect(
			BWAPI::Position target
		);

		virtual void onUnitDiscover(
			BWAPI::Unit unit
		);

		virtual void onUnitHide(
			BWAPI::Unit unit
		);

		virtual void onUnitCreate(
			BWAPI::Unit unit
		);

		virtual void onUnitDestroy(
			BWAPI::Unit unit
		);

		virtual void onUnitMorph(
			BWAPI::Unit unit
		);

		virtual void onUnitRenegade(
			BWAPI::Unit unit
		);

		virtual void onSaveGame(
			std::string gameName
		);
		// Everything below this line is safe to modify.

	private:
		GameManager _GameManager;	// Manager responsible for the game loop
};
