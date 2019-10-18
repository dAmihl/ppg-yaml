#pragma once
#include "PuzzGen.h"
#include "yaml-cpp/yaml.h"
#include <iostream>
#include <fstream>

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
#define RULES_LHS "lh"
#define RULES_RHS "rh"


class Yaml2Puzzle
{
public:
	Yaml2Puzzle();
	~Yaml2Puzzle();

	void setNumberOfNodes(int n);
	int getNumberOfNodes();

	Puzzle* generatePuzzle(std::string pathToYaml);

private:
	PuzzleObject* getObjectByName(std::string name, T_PuzzleObjectList objects);

	void log(std::string logStr, int logLevel);

	std::ofstream logFile;

	int numberOfNodes = 0;

};

