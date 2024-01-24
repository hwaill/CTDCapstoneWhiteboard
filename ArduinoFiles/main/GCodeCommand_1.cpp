#include "GCodeCommand.h"

GCodeCommand_1::GCodeCommand_1(GCodeCommandPair commandPairs[1]) : GCodeCommand() {
	_commandPairs = commandPairs;
	_numPairs = 1;
}