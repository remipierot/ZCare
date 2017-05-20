#include "ZCareAI.h"
#include <iostream>

using namespace BWAPI;
using namespace std;

//********** USED CALLBACKS **********//
void ZCareAI::onStart()
{
	// Enable the UserInput flag, which allows us to control the bot and type messages.
	Broodwar->enableFlag(Flag::UserInput);

	// Set the command optimization level so that common commands can be grouped
	// and reduce the bot's APM (Actions Per Minute).
	Broodwar->setCommandOptimizationLevel(2);

	// Set the local execution speed (useful for debug run)
	// On network, the highest local speed (lowest framerate) is used to run the game
	// LOCAL SPEED TABLE :
	//  5 LS = 200 FPS
	// 10 LS = 100 FPS
	// 20 LS =  50 FPS
	Broodwar->setLocalSpeed(10);

	// Load the build order
	_GameManager.initBuildOrder();
}

void ZCareAI::onFrame()
{
	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
	{
		return;
	}

	// Draw debug at each frame to avoid blinking drawings
	_GameManager.drawDebug();

	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0) 
	{
		return;
	}

	// Execute game loop
	_GameManager.update();
}

void ZCareAI::onSendText(string text)
{
	// Send the text to the game if it is not being processed.
	Broodwar->sendText("%s", text.c_str());

	// Make sure to use %s and pass the text as a parameter,
	// otherwise you may run into problems when you use the %(percent) character!
}

void ZCareAI::onReceiveText(Player player, string text)
{
	// Parse the received text
	Broodwar << player->getName() << " said \"" << text << "\"" << endl;
}

void ZCareAI::onUnitEvade(Unit unit)
{
	_GameManager.onUnitEvade(unit);
}

void ZCareAI::onUnitShow(Unit unit)
{
	_GameManager.onUnitShow(unit);
}

void ZCareAI::onUnitComplete(Unit unit)
{
	_GameManager.onUnitComplete(unit);
}
//********** END USED CALLBACKS **********//

//********** UNUSED CALLBACKS **********//
void ZCareAI::onEnd(bool isWinner)
{
}

void ZCareAI::onPlayerLeft(Player player)
{
}

void ZCareAI::onNukeDetect(Position target)
{
}

void ZCareAI::onUnitDiscover(Unit unit)
{
}

void ZCareAI::onUnitHide(Unit unit)
{
}

void ZCareAI::onUnitCreate(Unit unit)
{
}

void ZCareAI::onUnitDestroy(Unit unit)
{
}

void ZCareAI::onUnitMorph(Unit unit)
{
}

void ZCareAI::onUnitRenegade(Unit unit)
{
}

void ZCareAI::onSaveGame(string gameName)
{
}
//********** END UNUSED CALLBACKS **********//