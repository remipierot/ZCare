#include "BOParser.h"
#include "tinyxml2.h"

using namespace std;
using namespace BWAPI;

// Return the BO loaded with the given path
BuildOrder BOParser::loadBO(string fileName)
{
	BuildOrder buildOrder;
	const UnitType::set& allUnitTypes = UnitTypes::allUnitTypes();

	char result[MAX_PATH];
	GetModuleFileName(NULL, result, MAX_PATH);

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_ERROR_FILE_NOT_FOUND)
	{
		tinyxml2::XMLElement* root = doc.RootElement();
		tinyxml2::XMLElement* element = root->FirstChildElement();
		tinyxml2::XMLElement* sauvegarde = root->LastChildElement();

		BOInstruction* tmp;

		for (tinyxml2::XMLElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			// Create instruction here
			for (tinyxml2::XMLElement *elemChild = elem->FirstChildElement(); elemChild != NULL; elemChild = elemChild->NextSiblingElement())
			{
				std::string nomElement = elemChild->Value();
				const char*  text = elemChild->GetText();

				if (nomElement == "Type")
				{
					tmp = new BOInstruction(BOInstruction::strToType(text));
					tmp->setResearch(TechTypes::None);
					tmp->setUpgrade(UpgradeTypes::None);
					tmp->setUnit(UnitTypes::None, 0);
				}
				else if (nomElement == "SupplyCount")
				{
					tmp->setSupplyCount(atoi(text));
				}
				else if (nomElement == "Actions")
				{
					tmp->setUnit(UnitType::getType(text));
				}
				else if (nomElement == "Location")
				{
					tmp->setBaseIndex(atoi(text));
				}
				else if (nomElement == "Research")
				{
					tmp->setResearch(TechType::getType(text));
				}
				else if (nomElement == "Upgrade")
				{
					tmp->setUpgrade(UpgradeType::getType(text));
				}
			}

			buildOrder.addInstruction(tmp);
		}

		buildOrder.reset();
	}
	else
	{
		string s = "Unable to load " + fileName;
		Broodwar->sendText(s.c_str());
	}

	return 
		buildOrder;
}
