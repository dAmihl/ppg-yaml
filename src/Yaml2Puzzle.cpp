#include "Yaml2Puzzle.h"


void Yaml2Puzzle::setNumberOfNodes(int n)
{
	numberOfNodes = n;
}

int Yaml2Puzzle::getNumberOfNodes() const
{
	return numberOfNodes;
}


PPG::UPtr<PPG::Puzzle> Yaml2Puzzle::generatePuzzleByFile(PPG::Str pathToYaml)
{
	YAML::Node rootNode = YAML::LoadFile(pathToYaml);
	return generatePuzzle(rootNode);
}

PPG::UPtr<PPG::Puzzle> Yaml2Puzzle::generatePuzzleByString(PPG::Str yamlString)
{
	YAML::Node rootNode = YAML::Load(yamlString);
	return generatePuzzle(rootNode);
}


PPG::Ptr<PPG::Object> Yaml2Puzzle::getObjectByName(PPG::Str name, PPG::Vec<PPG::Ptr<PPG::Object>>& objects) {

	for (auto& obj : objects) {
		if (obj->getObjectName() == name) {
			return obj;
		}
	}

	return nullptr;
}

PPG::UPtr<PPG::Puzzle> Yaml2Puzzle::generatePuzzle(YAML::Node rootNode)
{

	PPG::Context c;

	PPG::Map<PPG::Str, PPG::State> statesMap;

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

						numTemplates = PPG::Randomizer::getRandomUintFromRange(minNum, maxNum);
					}
					else {
						numTemplates = 1;
					}
				}
			}
		}

		for (unsigned int i = 0; i < numTemplates; i++) {
			PPG::Vec<PPG::State> statesList;
			std::string objName;
			if (bIsTemplate) {
				objName = name + "_T_" + std::to_string(i);
				this->log("Generating template of " + name, 0);
			}
			else {
				objName = name;
				this->log(name + " is not a template. Generating object normal.", 0);
			}

			auto tmpObj = c.add<PPG::Object>(objName);
			if (bIsTemplate)
			{
				tmpObj->setTemplateName(objName);
			}
			
			bool defaultStateSet = false;
			for (const auto& s : states) {
				// TODO: Check unique state name
				std::string sName = s["name"].as<std::string>();
				bool bSetDefault = false;
				if (s[OBJECT_STATE_DEFAULT]) {
					bSetDefault = true;
				}
				PPG::State tmpS(sName);
				if (bSetDefault) {
					tmpObj->setCurrentState(tmpS);
					defaultStateSet = true;
				}
				statesMap[sName] = tmpS;
				statesList.push_back(tmpS);
			}

			if (!defaultStateSet) {
				// Choose random possible state as starting state
				PPG::State startState = PPG::Randomizer::getRandomFromList(statesList);
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
				}
				else {
					evName = ev.as<std::string>();
				}

				auto tmpEv = c.add<PPG::Event>(evName, tmpObj);

				if (ev["reversible"]) {
					bool isReversible = ev["reversible"].as<bool>();
					tmpEv->setIsReversible(isReversible);
					this->log("Event is reversible", 0);
				}
				else {
					this->log("Event not reversible", 0);
				}
			}

			PPG::StateTransition* objTransition = new PPG::StateTransition();

			for (const auto& ts : transitions) {
				objTransition->addTransition(ts["event"].as<std::string>(), (statesMap[ts["from"].as<std::string>()]), (statesMap[ts["to"].as<std::string>()]));
			}

			tmpObj->setStateTransition(*objTransition);
		}
	}


	// Parse Rules and generate ruleset
	if (rootNode[RULES_BLOCK]) {
		for (const auto& ru : rootNode[RULES_BLOCK]) {
			if (!ru[RULES_LHS] || !ru[RULES_RHS]) {
				this->log("Lefthandside of rule or righthandside of rule not found.", 1);
				continue;
			}

			if (!ru[RULES_TYPE]) {
				this->log("Ruletype of Rule not found.", 1);
				continue;
			}

			std::string lhName = ru[RULES_LHS]["name"].as<std::string>();
			std::string rhName = ru[RULES_RHS]["name"].as<std::string>();

			auto lhO = getObjectByName(lhName, c.getObjects());
			if (lhO == nullptr) {
				this->log("LH nullptr", 1);
				continue;
			}

			auto rhO = getObjectByName(rhName, c.getObjects());
			if (rhO == nullptr) {
				this->log("RH nullptr", 1);
				continue;
			};


			std::string lhStateName;
			std::string rhStateName;
			PPG::State lhState;
			PPG::State rhState;
			if (ru["lh"]["state"]) {
				lhStateName = ru[RULES_LHS]["state"].as<std::string>();
				lhState = statesMap[lhStateName];
			}
			if (ru["rh"]["state"]) {
				rhStateName = ru[RULES_RHS]["state"].as<std::string>();
				rhState = statesMap[rhStateName];
			}

			std::string ruleTypeName = ru[RULES_TYPE].as<std::string>();
			PPG::Rule::EPuzzleRuleType ruleType = PPG::Rule::EPuzzleRuleType::BEFORE;

			if (ruleTypeName == "BEFORE") {
				ruleType = PPG::Rule::EPuzzleRuleType::BEFORE;
			}
			else if (ruleTypeName == "STRICT_BEFORE") {
				ruleType = PPG::Rule::EPuzzleRuleType::STRICT_BEFORE;
			}
			else if (ruleTypeName == "AFTER") {
				ruleType = PPG::Rule::EPuzzleRuleType::AFTER;
			}
			else if (ruleTypeName == "STRICT_AFTER") {
				ruleType = PPG::Rule::EPuzzleRuleType::STRICT_AFTER;
			}
			else {
				this->log("Unknown Ruletype! Defaulting to BEFORE.", 1);
			}

			auto tmpRule = c.addRule(lhO, lhState, rhO, rhState, ruleType);
		}

		this->log("Rules generated:", 0);
		std::string out = "%%% Rules %%%\n";
		for (auto &it : c.getRules()) {
			out += it.getTextualRepresentation();
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

	PPG::WfcGenerator puzzGenerator(numberOfNodes);
	auto P = puzzGenerator.generatePuzzle(c);

	return P;
}

void Yaml2Puzzle::log(PPG::Str logStr, int logLevel)
{
#if WRITE_LOG > 0
	std::ofstream file("ppg-yaml.log", std::ofstream::app);
	file << logStr << "\n";
	file.close();
#endif
}
