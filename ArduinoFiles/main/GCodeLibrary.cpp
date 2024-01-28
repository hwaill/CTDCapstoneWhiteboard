#include "GCodeLibrary.h"

/* Indices for characters:

0 - 25: A - Z
26 - 51: a - z
52 - 61: 0 - 9


*/
const int numCommands[26] {
	15,
	28,
	21,
	19,
	17,
	13,
	26,
	13,
	5,
	13,
	13,
	9,
	17,
	13,
	24,
	17,
	28,
	21,
	23,
	9,
	13,
	9,
	17,
	9,
	10,
	13
};

const gcodeCommandString gc_cap_a[numCommands[0]] {
	{"G00 Z0.2",3},
	{"G00 X9.278739 Y16.814800",3},
	{"G01 Z-0.2 F10000.0",3},
	{"G01 X2.877939 Y0.000000 Z-0.2 F10000.000000",5},
	{"G00 Z0.2",2},
	{"G00 Z0.2",2},
	{"G00 X5.265539 Y5.588000",3},
	{"G01 Z-0.2 F10000.0",3},
	{"G01 X13.291939 Y5.588000 Z-0.2 F10000.000000",5},
	{"G00 Z0.2",2},
	{"G00 Z0.2",2},
	{"G00 X9.278739 Y16.814800",3},
	{"G01 Z-0.2 F10000.0",3},
	{"G01 X15.679539 Y0.000000 Z-0.2 F10000.000000",5},
	{"G00 Z0.2",2}
};

const gcodeCommandString * characters[26] = {
	gc_cap_a,
	gc_cap_b,
	gc_cap_c,
	gc_cap_d,
	gc_cap_e,
	gc_cap_f,
	gc_cap_g,
	gc_cap_h,
	gc_cap_i,
	gc_cap_j,
	gc_cap_k,
	gc_cap_l,
	gc_cap_m,
	gc_cap_n,
	gc_cap_o,
	gc_cap_p,
	gc_cap_q,
	gc_cap_r,
	gc_cap_s,
	gc_cap_t,
	gc_cap_u,
	gc_cap_v,
	gc_cap_w,
	gc_cap_x,
	gc_cap_y,
	gc_cap_z,
	};

GCodeLibrary::GCodeLibrary() {
	
}

void GCodeLibrary::generateAlphabet() {
	for(int i = 0; i < 1; i++) {
		//alphabetGroups[i] = _generateCommandGroupFromGCodeCharacter(alphabetGcode[i]);
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

GCodeCommand * GCodeLibrary::_generateCommandFromString(String input, int numPairs) {
	int spaceIndexes[8] = {};
	
	if(numPairs == 1) {
		GCodeCommandPair pairs[1];
		for(int i = 0; i < numPairs; i++) {
			pairs[i] = _generateCommandPairFromString(input);
		}
		
		return new GCodeCommand_1(pairs);

	} else if(numPairs == 2) {
		GCodeCommandPair pairs[2];
		for(int i = 0; i < numPairs; i++) {
			pairs[i] = _generateCommandPairFromString(input.substring(0, input.indexOf(' ')));
			if(i != numPairs - 1) {
				input = input.substring(0, input.indexOf(' '));
			}
		}
		return new GCodeCommand_2(pairs);
	
	} else if(numPairs == 3) {
		GCodeCommandPair pairs[3];
		for(int i = 0; i < numPairs; i++) {
			pairs[i] = _generateCommandPairFromString(input.substring(0, input.indexOf(' ')));
			if(i != numPairs - 1) {
				input = input.substring(input.indexOf(' ') + 1);
			}
		}
		return new GCodeCommand_3(pairs);
	} else if(numPairs == 4) {
		GCodeCommandPair pairs[4];
		for(int i = 0; i < numPairs; i++) {
			pairs[i] = _generateCommandPairFromString(input.substring(0, input.indexOf(' ')));
			if(i != numPairs - 1) {
				input = input.substring(input.indexOf(' ') + 1);
			}
		}
		return new GCodeCommand_4(pairs);
	} else if(numPairs == 5) {
		GCodeCommandPair pairs[5];
		for(int i = 0; i < numPairs; i++) {
			pairs[i] = _generateCommandPairFromString(input.substring(0, input.indexOf(' ')));
			if(i != numPairs - 1) {
				input = input.substring(input.indexOf(' ') + 1);
			}
		}
		return new GCodeCommand_5(pairs);
	} else if(numPairs == 6) {
		GCodeCommandPair pairs[6];
		for(int i = 0; i < numPairs; i++) {
			pairs[i] = _generateCommandPairFromString(input.substring(0, input.indexOf(' ')));
			if(i != numPairs - 1) {
				input = input.substring(input.indexOf(' ') + 1);
			}
		}
		return new GCodeCommand_6(pairs);
	} else if(numPairs == 7) {
		GCodeCommandPair pairs[7];
		for(int i = 0; i < numPairs; i++) {
			pairs[i] = _generateCommandPairFromString(input.substring(0, input.indexOf(' ')));
			if(i != numPairs - 1) {
				input = input.substring(input.indexOf(' ') + 1);
			}
		}
		return new GCodeCommand_7(pairs);
	} else if(numPairs == 8) {
		GCodeCommandPair pairs[8];
		for(int i = 0; i < numPairs; i++) {
			pairs[i] = _generateCommandPairFromString(input.substring(0, input.indexOf(' ')));
			if(i != numPairs - 1) {
				input = input.substring(input.indexOf(' ') + 1);
			}
		}
		return new GCodeCommand_8(pairs);
	}
}