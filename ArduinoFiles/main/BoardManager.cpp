#include "BoardManager.h"

BoardManager::BoardManager(GCodeHandler &myGCodeHandler) {
	_myGCodeHandler = &myGCodeHandler;
}