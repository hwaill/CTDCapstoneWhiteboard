#ifndef BoardManagerClass

#define BoardManagerClass
#include "Arduino.h"
#include "GCodeHandler.h"
#include <SD.h>
#include "RTC.h"

struct ToDoListItem {
	char* name;
};

class BoardManager {
	public:
		BoardManager(GCodeHandler &myGCodeHandler);
		void updateFromConfig();
	private:
		GCodeHandler* _myGCodeHandler;

		ToDoListItem _morningToDoList[20];
		ToDoListItem _dayToDoList[20];
		ToDoListItem _eveningToDoList[20];

		int _numMorningToDos;
		int _numDayToDos;
		int _numEveningToDos;
		
		int _theme;
		
};

#endif