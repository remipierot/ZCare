#include "BOParser.h"
#include "tinyxml2.h"

BOParser::BOParser(BuildOrder *buildOrder)
{
	bo = buildOrder;
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile("data/buildOrder.xml") != tinyxml2::XML_ERROR_FILE_NOT_FOUND)
	{
		std::cout << "Charge" << std::endl;
		tinyxml2::XMLElement* root = doc.RootElement();
		tinyxml2::XMLElement* element = root->FirstChildElement();
		tinyxml2::XMLElement* sauvegarde = root->LastChildElement();

		for (tinyxml2::XMLElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			//On crée l'instruction ici
			BOInstruction* tmp;

			
			for (tinyxml2::XMLElement *elemChild = elem->FirstChildElement(); elemChild != NULL; elemChild = elemChild->NextSiblingElement())
			{
				std::string nomElement = elemChild->Value();
				const char*  text = elemChild->GetText();

				if (nomElement == "Type")
				{
					tmp = new BOInstruction((BOInstruction::InstructionType)text);
				}
				else if (nomElement == "WorkerCount")
				{
					tmp->setWorkerCount((int)text);
				}
				else if (nomElement == "Actions")
				{
					tmp->setActions(UnitTypes::Zerg_Overlord);
				}
				bo->addInstruction(tmp);
			}
		}

	}
}


BOParser::~BOParser()
{
}
