#ifndef GCodeCommandClass
#define GCodeCommandClass
#include "GCodeCommandPair.h"

class GCodeCommand {
	public:
		GCodeCommand();
		
		String toString();
		String toStringAtCoordinate(double x, double y);
		GCodeCommandPair *_commandPairs;
		int _numPairs;
};

class GCodeCommand_1 : public GCodeCommand {
	public:
		GCodeCommand_1(GCodeCommandPair commandPairs[1]);
};

class GCodeCommand_2 : public GCodeCommand {
	public:
		GCodeCommand_2(GCodeCommandPair commandPairs[2]);
};

class GCodeCommand_3 : public GCodeCommand {
	public:
		GCodeCommand_3(GCodeCommandPair commandPairs[3]);
};

class GCodeCommand_4 : public GCodeCommand {
	public:
		GCodeCommand_4(GCodeCommandPair commandPairs[4]);
};

class GCodeCommand_5 : public GCodeCommand {
	public:
		GCodeCommand_5(GCodeCommandPair commandPairs[5]);
};

class GCodeCommand_6 : public GCodeCommand {
	public:
		GCodeCommand_6(GCodeCommandPair commandPairs[6]);
};

class GCodeCommand_7 : public GCodeCommand {
	public:
		GCodeCommand_7(GCodeCommandPair commandPairs[7]);
};

class GCodeCommand_8 : public GCodeCommand {
	public:
		GCodeCommand_8(GCodeCommandPair commandPairs[8]);
};

#endif