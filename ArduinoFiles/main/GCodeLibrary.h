#ifndef GCodeLibraryClass
#define GCodeLibraryClass

#include "Arduino.h"
#include "GCodeCommandGroup.h"
#include "GCodeCommand.h"
#include "GCodeCommandPair.h"

struct gcodeCommandString {
	String commandString;
	int numPairs;
};

class GCodeLibrary {
	public:
		GCodeLibrary();

		void generateAlphabet();
		GCodeCommandGroup alphabetGroups[52];
		GCodeCommandPair _generateCommandPairFromString(String input);
		GCodeCommand * _generateCommandFromString(String input, int numPairs);
	private:
		void _generateAlphabet();
		// GCodeCommandPair _generateCommandPairFromString(String input);
		// GCodeCommand _generateCommandFromString(String input, int numPairs);
		GCodeCommandGroup _generateCommandGroupFromGCodeCharacter(gcodeCharacter);
};

#endif