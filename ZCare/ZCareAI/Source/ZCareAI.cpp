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
	Broodwar->setLocalSpeed(10);

	_GameManager.initBO();
	this->setCombatManager(_GameManager.getCombatManager());
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
	_GameManager.drawDebug();

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

	int y = 60;
	for (Squad* squad : _CombatManager->getSquadList())
	{
		Broodwar->drawTextScreen(10, y, "Squad %d, Nombre : %d", squad->getIdSquad(), squad->numberUnit());
		y += 20;
	}

	_CombatManager->setUnitDiscover(&unitShow);

	for (Unit uni : unitShow)
	{
		Broodwar->drawCircleMap(uni->getPosition(),10, Color(0, 60, 0), true);
	}
	/*for (Unit building : buildingShow)
	{
		Broodwar->drawCircleMap(building->getPosition(), 10, Color(0, 0, 60), true);
	}*/
	
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
	bool notOurUnit = true;
	if (!unit->getType().isNeutral())
	{
		for (Unit usUnit : Broodwar->self()->getUnits())
		{
			if (usUnit == unit)
			{
				notOurUnit = false;
				break;
			}

		}

		if (notOurUnit)
			unitShow.insert(unit);
			//if (unit->getType().isBuilding())
			//	buildingShow.insert(unit);
			//else unitShow.insert(unit);
	}
}

void ZCareAI::onUnitHide(BWAPI::Unit unit)
{
}

void ZCareAI::onUnitCreate(BWAPI::Unit unit)
{
	UnitType type = unit->getType();
	if (!type.isWorker() && !type.isBuilding() && unit->canAttack())
	{
		int squadNumber = _CombatManager->squadNumber();
		Squad* squad = 0;
		if (squadNumber == 0)
		{
			squadNumber = 1;
			squad = new Squad(squadNumber);
			squad->setModeSquad(Squad::attackMode);
			BWAPI::Position pos;

			for (Base *base : *_CombatManager->getBase())
			{
				if (base->isEnnemyLocation && base->isStartingLocation)
				{
					pos = base->position;
					break;
				}
			}
			squad->setPositionObjective(pos);
			_CombatManager->addSquad(squad);
		}		

		bool insertion = false;
		int currentSquad = 1;
		while (!insertion)
		{
			squad = _CombatManager->findSquad(currentSquad);
			if (squad != 0)
			{
				//Broodwar->drawTextScreen(10, 60, "Squad %d : %d", currentSquad, _CombatManager->findSquad(currentSquad)->numberUnit());
				if (squad->insertUnit(unit))
				{
					insertion = true;
				}
				else currentSquad += 1;
			}
				
			else
			{
				squad = new Squad(currentSquad);
				squad->setModeSquad(Squad::attackMode);
				BWAPI::Position pos;
				for (Base *base : *_CombatManager->getBase())
				{
					if (base->isEnnemyLocation && base->isStartingLocation)
					{
						pos = base->position;
						break;
					}
				}
				_CombatManager->addSquad(squad);		
			}
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

void ZCareAI::setCombatManager(CombatManager *combatManager)
{
	this->_CombatManager = combatManager;
}