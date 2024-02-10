#ifndef GCodeHandlerClass

#define GCodeHandlerClass
#include "Arduino.h"
#include "GCodeLibrary.cpp"

class GCodeHandler {
	public:
		GCodeHandler(Stream &gcodeSerial, Stream &consoleSerial);

		void initialize();

		void sendSingleGCODE(String command);
		void sendMultipleGCODE(const char* commands[], int numCommands);

		String mapGCODEToPositionAndScale(const char* command, double posX, double posY, double scale);
		void sendCharacterAtPositionAndScale(const char* commands[], double posX, double posY, double scale);
		
		void drawLine(double startX, double startY, double endX, double endY);
		void drawRect(double startX, double startY, double endX, double endY);
		void drawCircle(double centerX, double centerY, double radius);
	private:
		Stream* _gcodeSerial;
		Stream* _consoleSerial;
		
		float _cursorX;
		float _cursorY;

		static double _CANVAS_START_X;
		static double _CANVAS_START_Y;
		static double _CANVAS_END_X;
		static double _CANVAS_END_Y;

		static String _SENT_HEADER;
		static String _RECV_HEADER;

		String _waitGRBLSerial();
		void _emptyGRBLSerialBuffer();
		void _wakeGRBLSerial();
		int _mapCharToIndex(const char c);
};

#endif