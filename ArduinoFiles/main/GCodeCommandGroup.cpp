#include "GCodeCommandGroup.h"

GCodeCommandGroup::GCodeCommandGroup(GCodeCommand *commands, int numCommands) {
	_commands = commands;
	_numCommands = numCommands;
}

GCodeCommand* GCodeCommandGroup::getCommands() {
	return _commands;
}

int GCodeCommandGroup::getNumCommands() {
	return _numCommands;
}