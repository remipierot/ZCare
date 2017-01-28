#include "ZCareAI.h"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

void ZCareAI::onStart()
{
	// Hello World!
	Broodwar->sendText("Préparez vous à mourir!");

	// Enable the UserInput flag, which allows us to control the bot and type messages.
	Broodwar->enableFlag(Flag::UserInput);

	// Set the command optimization level so that common commands can be grouped
	// and reduce the bot's APM (Actions Per Minute).
	Broodwar->setCommandOptimizationLevel(2);

	// Set the local execution speed (useful for debug run)
	Broodwar->setLocalSpeed(20);
}

void ZCareAI::onEnd(bool isWinner)
{
	// Called when the game ends
	if (isWinner)
	{
		// Log your win here!
	}
}

void ZCareAI::onFrame()
{
	// Called once every game frame
	// Display the game frame rate as text in the upper left area of the screen
	Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());

	Broodwar->drawTextScreen(0, 30, "Scouting locations data :");
	Broodwar->drawTextScreen(10, 40, "To Scout : %d", _GameManager.toScoutCount());
	Broodwar->drawTextScreen(10, 50, "Scouted : %d", _GameManager.scoutedCount());
	Broodwar->drawTextScreen(10, 60, "Unscouted : %d", _GameManager.unscoutedCount());
	Broodwar->drawTextScreen(10, 70, "Being Scout : %d", _GameManager.beingScoutCount());
	Broodwar->drawTextScreen(0, 90, "Scouting units data :");
	Broodwar->drawTextScreen(10, 100, "Scouts : %d", _GameManager.scoutsCount());
	Broodwar->drawTextScreen(10, 110, "Busy Scouts : %d", _GameManager.busyScoutsCount());

	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
	{
		return;
	}

	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
	{
		return;
	}

	// Game loop
	_GameManager.update();

  // Iterate through all the units that we own
  for (auto &u : Broodwar->self()->getUnits())
  {
	// Finally make the unit do some stuff!


	// If the unit is a worker unit
	if ( u->getType().isWorker() )
	{
	  // if our worker is idle
	  if ( u->isIdle() )
	  {
		// Order workers carrying a resource to return them to the center,
		// otherwise find a mineral patch to harvest.
		if ( u->isCarryingGas() || u->isCarryingMinerals() )
		{
		  u->returnCargo();
		}
		else if ( !u->getPowerUp() )  // The worker cannot harvest anything if it
		{                             // is carrying a powerup such as a flag
		  // Harvest from the nearest mineral patch or gas refinery
		  if ( !u->gather( u->getClosestUnit( IsMineralField || IsRefinery )) )
		  {
			// If the call fails, then print the last error message
			Broodwar << Broodwar->getLastError() << std::endl;
		  }

		} // closure: has no powerup
	  } // closure: if idle

	}
	else if ( u->getType().isResourceDepot() ) // A resource depot is a Command Center, Nexus, or Hatchery
	{

	  // Order the depot to construct more workers! But only when it is idle.
	  if ( u->isIdle() && !u->train(u->getType().getRace().getSupplyProvider()) )
	  {
		  /*
		// If that fails, draw the error at the location so that you can visibly see what went wrong!
		// However, drawing the error once will only appear for a single frame
		// so create an event that keeps it on the screen for some frames
		Position pos = u->getPosition();
		Error lastErr = Broodwar->getLastError();
		Broodwar->registerEvent([pos,lastErr](Game*){ Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str()); },   // action
								nullptr,    // condition
								Broodwar->getLatencyFrames());  // frames to run

		// Retrieve the supply provider type in the case that we have run out of supplies
		UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
		static int lastChecked = 0;

		// If we are supply blocked and haven't tried constructing more recently
		if (  lastErr == Errors::Insufficient_Supply &&
			  lastChecked + 400 < Broodwar->getFrameCount() &&
			  Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0 )
		{
		  lastChecked = Broodwar->getFrameCount();

		  // Retrieve a unit that is capable of constructing the supply needed
		  Unit supplyBuilder = u->getClosestUnit(  GetType == supplyProviderType.whatBuilds().first &&
													(IsIdle || IsGatheringMinerals) &&
													IsOwned);
		  // If a unit was found
		  if ( supplyBuilder )
		  {
			if ( supplyProviderType.isBuilding() )
			{
			  TilePosition targetBuildLocation = Broodwar->getBuildLocation(supplyProviderType, supplyBuilder->getTilePosition());
			  if ( targetBuildLocation )
			  {
				// Register an event that draws the target build location
				Broodwar->registerEvent([targetBuildLocation,supplyProviderType](Game*)
										{
										  Broodwar->drawBoxMap( Position(targetBuildLocation),
																Position(targetBuildLocation + supplyProviderType.tileSize()),
																Colors::Blue);
										},
										nullptr,  // condition
										supplyProviderType.buildTime() + 100 );  // frames to run

				// Order the builder to construct the supply structure
				supplyBuilder->build( supplyProviderType, targetBuildLocation );
			  }
			}
			else
			{
			  // Train the supply provider (Overlord) if the provider is not a structure
			  supplyBuilder->train( supplyProviderType );
			}
		  } // closure: supplyBuilder is valid
		} // closure: insufficient supply
		*/
	  }
	  

	}

  } // closure: unit iterator
}

void ZCareAI::onSendText(std::string text)
{
	// Send the text to the game if it is not being processed.
	Broodwar->sendText("%s", text.c_str());

	// Make sure to use %s and pass the text as a parameter,
	// otherwise you may run into problems when you use the %(percent) character!
}

void ZCareAI::onReceiveText(BWAPI::Player player, std::string text)
{
	// Parse the received text
	Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void ZCareAI::onPlayerLeft(BWAPI::Player player)
{
	// Interact verbally with the other players in the game by
	// announcing that the other player has left.
	Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void ZCareAI::onNukeDetect(BWAPI::Position target)
{
	// Check if the target is a valid position
	if (target)
	{
		// if so, print the location of the nuclear strike target
		Broodwar << "Nuclear Launch Detected at " << target << std::endl;
	}
	else 
	{
		// Otherwise, ask other players where the nuke is!
		Broodwar->sendText("Where's the nuke?");
	}
	// You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void ZCareAI::onUnitDiscover(BWAPI::Unit unit)
{
}

void ZCareAI::onUnitEvade(BWAPI::Unit unit)
{
}

void ZCareAI::onUnitShow(BWAPI::Unit unit)
{
}

void ZCareAI::onUnitHide(BWAPI::Unit unit)
{
}

void ZCareAI::onUnitCreate(BWAPI::Unit unit)
{
	if ( Broodwar->isReplay() )
	{
		// if we are in a replay, then we will print out the build order of the structures
		if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			int seconds = Broodwar->getFrameCount()/24;
			int minutes = seconds/60;
			seconds %= 60;
			Broodwar->sendText("%.2d:%.2d: %s creates a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
		}
	}
}

void ZCareAI::onUnitDestroy(BWAPI::Unit unit)
{
}

void ZCareAI::onUnitMorph(BWAPI::Unit unit)
{
	if (Broodwar->isReplay())
	{
		// if we are in a replay, then we will print out the build order of the structures
		if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			int seconds = Broodwar->getFrameCount()/24;
			int minutes = seconds/60;
			seconds %= 60;
			Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
		}
	}
}

void ZCareAI::onUnitRenegade(BWAPI::Unit unit)
{
}

void ZCareAI::onSaveGame(std::string gameName)
{
  Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void ZCareAI::onUnitComplete(BWAPI::Unit unit)
{
}

bool checkStateUnit(BWAPI::Unit unit)
{
	// Ignore the unit if it no longer exists
	// Make sure to include this block when handling any Unit pointer!
	if (!unit->exists())
		return false;

	// Ignore the unit if it has one of the following status ailments
	if (unit->isLockedDown() || unit->isMaelstrommed() || unit->isStasised())
		return false;

	// Ignore the unit if it is in one of the following states
	if (unit->isLoaded() || !unit->isPowered() || unit->isStuck())
		return false;

	// Ignore the unit if it is incomplete or busy constructing
	if (!unit->isCompleted() || unit->isConstructing())
		return false;

	return true;
}