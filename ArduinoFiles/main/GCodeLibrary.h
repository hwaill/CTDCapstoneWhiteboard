#ifndef GCodeLibraryClass
#define GCodeLibraryClass

#include "Arduino.h"
#include "GCodeCommandGroup.h"

class GCodeLibrary {
	public:
		GCodeLibrary();

		static GCodeCommandGroup gc_A;
};

#endif