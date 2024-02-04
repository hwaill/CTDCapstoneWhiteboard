#ifndef GCodeHandlerClass

#define GCodeHandlerClass
#include "Arduino.h"
#include "GCodeLibrary.cpp"

class GCodeHandler {
	public:
		GCodeHandler(Stream &gcodeSerial, Stream &consoleSerial);

		void sendSingleGCODE(String command);
		void sendMultipleGCODE(char* commands[], int numCommands);
		void sendMultipleGCODE(const char* commands[], int numCommands);

		String mapGCODEToPositionAndScale(gcodeCommandString command, double posX, double posY, double scale);
		void sendCharacterAtPositionAndScale(gcodeCommandString commands[], int numCommands, double posX, double posY, double scale);
		
		void drawLine(double startX, double startY, double endX, double endY);
		void drawRect(double startX, double startY, double endX, double endY);
		void drawCircle(double centerX, double centerY, double radius);
		
		void initialize();
	private:
		float _cursorX;
		float _cursorY;
		Stream* _gcodeSerial;
		Stream* _consoleSerial;

		static String _SENT_HEADER;
		static String _RECV_HEADER;
		String _waitGRBLSerial();
		void _emptyGRBLSerialBuffer();
		void _wakeGRBLSerial();
		int _mapCharToIndex(char c);
};

#endif