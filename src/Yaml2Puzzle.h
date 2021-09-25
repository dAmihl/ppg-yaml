#pragma once
#include "PuzzGen.h"
#include "yaml-cpp/yaml.h"

#define WRITE_LOG 0
#if WRITE_LOG > 0
#include <iostream>
#include <fstream>
#endif

#define NUM_NODES "num_nodes"

#define OBJECTS_BLOCK "objects"

#define OBJECT_TEMPLATE "is_template"
#define OBJECT_TEMPLATE_EXACT_NUM "exact_number"
#define OBJECT_TEMPLATE_MIN_NUM "min_number"
#define OBJECT_TEMPLATE_MAX_NUM "max_number"

#define OBJECT_STATES "states"
#define OBJECT_EVENTS "events"
#define OBJECT_TRANSITIONS "transitions"

#define OBJECT_STATE_DEFAULT "default"

#define RULES_BLOCK "rules"
#define RULES_TYPE "type"
#define RULES_LHS "lh"
#define RULES_RHS "rh"

class Yaml2Puzzle
{

public:
	void setNumberOfNodes(int n);
	int getNumberOfNodes() const;

	PPG::UPtr<PPG::Puzzle> generatePuzzleByFile(PPG::Str pathToYaml);
	PPG::UPtr<PPG::Puzzle> generatePuzzleByString(PPG::Str yamlString);

private:
	PPG::Ptr<PPG::Object> getObjectByName(PPG::Str name, PPG::Vec<PPG::Ptr<PPG::Object>>& objects);

	PPG::UPtr<PPG::Puzzle> generatePuzzle(YAML::Node rootNode);

	void log(PPG::Str logStr, int logLevel);

	std::ofstream logFile;

	int numberOfNodes = 0;

};

