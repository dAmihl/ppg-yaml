#include "Yaml2Puzzle.h"


Yaml2Puzzle::Yaml2Puzzle()
{
}


Yaml2Puzzle::~Yaml2Puzzle()
{
}

void Yaml2Puzzle::setNumberOfNodes(int n)
{
	this->numberOfNodes = n;
}

int Yaml2Puzzle::getNumberOfNodes()
{
	return this->numberOfNodes;
}


Puzzle * Yaml2Puzzle::generatePuzzle(std::string pathToYaml)
{

	YAML::Node rootNode = YAML::LoadFile(pathToYaml);

	T_PuzzleEventList eventsList;
	T_PuzzleObjectList objectsList;
	T_PuzzleRuleList rulesList;
	std::map<std::string, PuzzleState*> statesMap;

	// Parse objects and generate object set
	for (const auto& obj : rootNode[OBJECTS_BLOCK]) {

		std::string name = obj["name"].as<std::string>();
		YAML::Node states = obj[OBJECT_STATES];
		YAML::Node events = obj[OBJECT_EVENTS];
		YAML::Node transitions = obj[OBJECT_TRANSITIONS];

		bool bIsTemplate = false;

		unsigned int numTemplates = 1;

		if (obj[OBJECT_TEMPLATE]) {
			bIsTemplate = obj[OBJECT_TEMPLATE].as<bool>();

			if (bIsTemplate) {
				this->log(name + " Is a template.", 0);
				unsigned int exactNum = 0;
				unsigned int minNum = 0;
				unsigned int maxNum = 0;

				if (obj[OBJECT_TEMPLATE_EXACT_NUM]) {
					exactNum = obj[OBJECT_TEMPLATE_EXACT_NUM].as<unsigned int>();
					numTemplates = exactNum > 1 ? exactNum : 1;
				}

				if (!exactNum) {
					if (obj[OBJECT_TEMPLATE_MIN_NUM] && obj[OBJECT_TEMPLATE_MAX_NUM]) {
						minNum = obj[OBJECT_TEMPLATE_MIN_NUM].as<unsigned int>();
						maxNum = obj[OBJECT_TEMPLATE_MAX_NUM].as<unsigned int>();

						numTemplates = PuzzleRandomizer::getRandomUintFromRange(minNum, maxNum);
					}
					else {
						numTemplates = 1;
					}
				}
			}
		}

		for (unsigned int i = 0; i < numTemplates; i++) {
			PuzzleObject* tmpObj;
			PuzzleList<PuzzleState>::Type statesList;
			if (bIsTemplate) {
				tmpObj = new PuzzleObject(name + "_T_"+std::to_string(i));
				tmpObj->setTemplateName(name);
				this->log("Generating template of " + name, 0);
			}
			else {
				tmpObj = new PuzzleObject(name);
				this->log(name + " is not a template. Generating object normal.", 0);
			}

			bool defaultStateSet = false;
			for (const auto& s : states) {
				// TODO: Check unique state name
				std::string sName = s["name"].as<std::string>();
				bool bSetDefault = false;
				if (s[OBJECT_STATE_DEFAULT]) {
					bSetDefault = true;
				}
				PuzzleState* tmpS = new PuzzleState(sName);
				if (bSetDefault) {
					tmpObj->setCurrentState(*tmpS);
					defaultStateSet = true;
				}
				statesMap[sName] = tmpS;
				statesList.push_back(*tmpS);
			}

			if (!defaultStateSet) {
				// Choose random possible state as starting state
				PuzzleState startState = PuzzleRandomizer::getRandomStateFromList(statesList);
				tmpObj->setCurrentState(startState);
				this->log("Set random default state", 0);
			}
			else {
				this->log("Set default state DEFAULT", 0);
			}

			for (const auto& ev : events) {
				std::string evName;
				
				if (ev["name"]) {
					evName = ev["name"].as<std::string>();	
				} else {
					evName = ev.as<std::string>();
				}

				PuzzleEvent* tmpEv = new PuzzleEvent(evName, tmpObj);

				if (ev["reversible"]) {
					bool isReversible = ev["reversible"].as<bool>();
					tmpEv->setIsReversible(isReversible);
					this->log("Event is reversible", 0);
				}
				else {
					this->log("Event not reversible", 0);
				}

				eventsList.push_back(tmpEv);
			}

			StateTransition* objTransition = new StateTransition();

			for (const auto& ts : transitions) {
				objTransition->addTransition(ts["event"].as<std::string>(), *(statesMap[ts["from"].as<std::string>()]), *(statesMap[ts["to"].as<std::string>()]));
			}

			tmpObj->setStateTransition(*objTransition);

			objectsList.push_back(tmpObj);
		}
	}

		

	// Parse Rules and generate ruleset
	if (rootNode[RULES_BLOCK]) {
		for (const auto& ru : rootNode[RULES_BLOCK]) {

			if (!ru[RULES_LHS] || !ru[RULES_RHS]) {
				this->log("Lefthandside of rule or righthandside of rule not found.", 1);
				continue;
			}
			
			std::string lhName = ru[RULES_LHS]["name"].as<std::string>();
			std::string rhName = ru[RULES_RHS]["name"].as<std::string>();

			PuzzleObject* lhO = this->getObjectByName(lhName, objectsList);
			if (lhO == nullptr) {
				this->log("LH nullptr", 1);
				continue;
			}

			PuzzleObject* rhO = this->getObjectByName(rhName, objectsList);
			if (rhO == nullptr) {
				this->log("RH nullptr", 1);
				continue;
			};


			std::string lhStateName;
			std::string rhStateName;
			PuzzleState* lhState = nullptr;
			PuzzleState* rhState = nullptr;
			if (ru["lh"]["state"]) {
				lhStateName = ru[RULES_LHS]["state"].as<std::string>();
				lhState = statesMap[lhStateName];
			}
			if (ru["rh"]["state"]) {
				rhStateName = ru[RULES_RHS]["state"].as<std::string>();
				rhState = statesMap[rhStateName];
			}


			PuzzleRule* tmpRule = new PuzzleRule(lhO, lhState, rhO, rhState, PuzzleRule::E_PuzzleRuleType::BEFORE);
			rulesList.push_back(*tmpRule);
		}

		this->log("Rules generated:", 0);
		std::string out = "%%% Rules %%%\n";
		for (T_PuzzleRuleList::iterator it = rulesList.begin(); it != rulesList.end(); ++it) {
			out += (*it).getTextualRepresentation();
			out += "\n";
		}
		this->log(out, 0);
	}
	else {
		this->log("No rules found.", 0);
	}

	if (rootNode[NUM_NODES]) {
		int numNodesYaml = rootNode[NUM_NODES].as<int>();
		this->setNumberOfNodes(numNodesYaml);
		this->log("Number of nodes read from config.", 0);
	}
	else {
		this->log("No number of nodes found in config. Using standard value.", 0);
	}

	PuzzleGenerator * puzzGenerator = new PuzzleGenerator();
	puzzGenerator->setNumberNodes(this->numberOfNodes);
	Puzzle *P = puzzGenerator->generatePuzzle(objectsList, eventsList, rulesList);


	return P;
}


PuzzleObject* Yaml2Puzzle::getObjectByName(std::string name, T_PuzzleObjectList objects) {

	for (const auto& obj : objects) {
		if (obj->getObjectName() == name) {
			return obj;
		}
	}

	return nullptr;
}

void Yaml2Puzzle::log(std::string logStr, int logLevel)
{
	std::ofstream file("ppg-yaml.log", std::ofstream::app);
	file << logStr << "\n";
	file.close();
}
