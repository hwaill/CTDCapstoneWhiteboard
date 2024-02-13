#ifndef GCodeLibraryClass
#define GCodeLibraryClass

#include "Arduino.h"
#include "GCodeCommandGroup.h"
#include "GCodeCommand.h"
#include "GCodeCommandPair.h"

struct gcodeCharacter {
	String text;
	int numCommands;
	int numPairs[];
};

class GCodeLibrary {
	public:
		GCodeLibrary();

		void generateAlphabet();
		GCodeCommandGroup alphabetGroups[52];
	private:
		void _generateAlphabet();
		GCodeCommandPair _generateCommandPairFromString(String input);
		GCodeCommand _generateCommandFromString(String input, int numPairs);
		GCodeCommandGroup _generateCommandGroupFromGCodeCharacter(gcodeCharacter);
};

#endif