#include "BOParser.h"
#include "tinyxml2.h"

using namespace std;
using namespace BWAPI;

BOParser::BOParser()
{

}

BOParser::BOParser(BuildOrder *buildOrder)
{
	bo = buildOrder;
	const UnitType::set& allUnitTypes = UnitTypes::allUnitTypes();

	Broodwar->sendText("Trying to load BO");

	char result[MAX_PATH];
	GetModuleFileName(NULL, result, MAX_PATH);
	Broodwar->sendText("DLL running at : %s", result);

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile("bwapi-data/data/buildOrder.xml") != tinyxml2::XML_ERROR_FILE_NOT_FOUND)
	{
		Broodwar->sendText("BO loading");
		std::cout << "Charge" << std::endl;
		tinyxml2::XMLElement* root = doc.RootElement();
		tinyxml2::XMLElement* element = root->FirstChildElement();
		tinyxml2::XMLElement* sauvegarde = root->LastChildElement();

		BOInstruction* tmp;
		int nbInstruction = 0;

		for (tinyxml2::XMLElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			nbInstruction++;
			//On crée l'instruction ici
			for (tinyxml2::XMLElement *elemChild = elem->FirstChildElement(); elemChild != NULL; elemChild = elemChild->NextSiblingElement())
			{
				std::string nomElement = elemChild->Value();
				const char*  text = elemChild->GetText();

				if (nomElement == "Type")
				{
					tmp = new BOInstruction(BOInstruction::strToType(text));
				}
				else if (nomElement == "WorkerCount")
				{
					tmp->setWorkerCount(atoi(text));
				}
				else if (nomElement == "Actions")
				{
					tmp->setActions(UnitType::getType(text));
				}

				bo->addInstruction(tmp);
			}
		}

		Broodwar->sendText("%d instructions loaded", nbInstruction);
	}
}
