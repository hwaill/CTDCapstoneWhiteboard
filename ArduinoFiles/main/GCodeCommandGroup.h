#ifndef GCodeCommandGroupClass
#define GCodeCommandGroupClass

#include "GCodeCommand.h"

class GCodeCommandGroup {
	public:
		GCodeCommandGroup(GCodeCommand *commands, int numCommands);

		GCodeCommand* getCommands();
		int getNumCommands();

	private:
		GCodeCommand *_commands;
		int _numCommands;
};

#endif