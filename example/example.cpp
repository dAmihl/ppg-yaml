
#include <iostream>

#include "Yaml2Puzzle.h"

int main()
{

	Yaml2Puzzle y2p;

	auto P = y2p.generatePuzzleByFile("universe1.yaml");

	std::cout << P->getExtendedTextualRepresentation() << "\n";

	auto G = P->getGraphRepresentation();

	return 0;
}