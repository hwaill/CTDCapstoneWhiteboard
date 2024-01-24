#include "GCodeLibrary.h"

const gcodeCharacter alphabetGcode[52] = {
	{"G00 Z0.2,G00 X9.278739 Y16.814800,G01 Z-0.2 F10000.0,G01 X2.877939 Y0.000000 Z-0.2 F10000.000000,G00 Z0.2,G00 Z0.2,G00 X5.265539 Y5.588000,G01 Z-0.2 F10000.0,G01 X13.291939 Y5.588000 Z-0.2 F10000.000000,G00 Z0.2,G00 Z0.2,G00 X9.278739 Y16.814800,G01 Z-0.2 F10000.0,G01 X15.679539 Y0.000000 Z-0.2 F10000.000000,G00 Z0.2", 15, {2,3,3,5,2,2,3,3,5,2,2,3,3,5,2}},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}
};

const gcodeCharacter gc_A = {"G00 Z0.2,G00 X9.278739 Y16.814800,G01 Z-0.2 F10000.0,G01 X2.877939 Y0.000000 Z-0.2 F10000.000000,G00 Z0.2,G00 Z0.2,G00 X5.265539 Y5.588000,G01 Z-0.2 F10000.0,G01 X13.291939 Y5.588000 Z-0.2 F10000.000000,G00 Z0.2,G00 Z0.2,G00 X9.278739 Y16.814800,G01 Z-0.2 F10000.0,G01 X15.679539 Y0.000000 Z-0.2 F10000.000000,G00 Z0.2", 15, {2,3,3,5,2,2,3,3,5,2,2,3,3,5,2}};

GCodeLibrary::GCodeLibrary() {

}

void GCodeLibrary::generateAlphabet() {
	for(int i = 0; i < 1; i++) {
		alphabetGroups[i] = _generateCommandGroupFromGCodeCharacter(alphabetGcode[i]);
	}
}

GCodeCommandPair GCodeLibrary::_generateCommandPairFromString(String input) {
	String type = input.substring(0, 1);
	double value;
	if(input.substring(1, 2).equals("-")) {
		value = input.substring(2).toDouble() * -1.0;
	} else {
		value = input.substring(1).toDouble();
	}

	GCodeCommandPair toReturn(type, value);

	return toReturn;
}

GCodeCommand GCodeLibrary::_generateCommandFromString(String input, int numPairs) {
	int spaceIndexes[8] = {};
	
	if(numPairs == 1) {
		GCodeCommandPair pairs[1];
		pairs[0] = _generateCommandPairFromString(input);
		GCodeCommand_1 toReturn()
	} else if(numPairs == 2) {

	} else if(numPairs == 3) {
		
	} else if(numPairs == 4) {
		
	} else if(numPairs == 5) {
		
	} else if(numPairs == 6) {
		
	} else if(numPairs == 7) {
		
	} else if(numPairs == 8) {
		
	}
}

GCodeCommandGroup GCodeLibrary::_generateCommandGroupFromGCodeCharacter(gcodeCharacter) {

}