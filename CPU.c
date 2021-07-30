#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct registers {
	struct {
		union {
			struct {
				unsigned char A;
				unsigned char F;
			};
			unsigned short AF;
		};
	};

	struct {
		union {
			struct {
				unsigned char B;
				unsigned char C;
			};
			unsigned short BC;
		};
	};

	struct {
		union {
			struct {
				unsigned char D;
				unsigned char E;
			};
			unsigned short DE;
		};
	};
  struct {
    union {
      struct {
				unsigned char L;
        unsigned char H;
      };
      unsigned short HL;
    };
	};

	//Shadow registers
	struct {
		union {
			struct {
				unsigned char Ashadow;
				unsigned char Fshadow;
			};
			unsigned short AFshadow;
		};
	};

	struct {
		union {
			struct {
				unsigned char Bshadow;
				unsigned char Cshadow;
			};
			unsigned short BCshadow;
		};
	};

	struct {
		union {
			struct {
				unsigned char Dshadow;
				unsigned char Eshadow;
			};
			unsigned short DEshadow;
		};
	};

  struct {
    union {
      struct {
        unsigned char Hshadow;
        unsigned char Lshadow;
      };
      unsigned short HLshadow;
    };
	};

	unsigned short IX;
	unsigned short IY;

	//Stack pointer
	struct {
    union {
      struct {
        unsigned char S;
        unsigned char P;
      };
      unsigned short SP;
    };
	};


	unsigned char I;
	unsigned char R;
	unsigned short PC;


} extern registers;


unsigned char memory[0xFFFF];
struct registers R;
int (*InstructionTable[255])();

void reset() {
	memset(memory, 0, 1 << 15);
	R.AF = 0;
	R.BC = 0;
	R.DE = 0;
	R.HL = 0;
	R.AFshadow = 0;
	R.BCshadow = 0;
	R.DEshadow = 0;
	R.HLshadow = 0;
	R.IX = 0;
	R.IY = 0;
	R.SP = 0xFFFF;
	R.I = 0;
	R.R = 0;
	R.PC = 0;
}

int step() {
	unsigned char instruction = memory[R.PC];
	R.PC++;
	printf("\n\n\n\n");
	printf("         ");
	return InstructionTable[instruction]();
}

void printMemoryBlock(int startAddress) {
	int blockSize = 16;
	int rows = 8;
	for(int i = 0; i < rows; i++) {
		printf("0x%04X  |   ", startAddress + (i * blockSize));
		for(int j = 0; j < blockSize; j++) {
			printf("%02X", memory[startAddress + (i * blockSize) + j]);
			if(!((j + 1) % 4)) {
				printf(" ");
			}
		}

		printf("  |  ");

		for(int j = 0; j < blockSize; j++) {
			unsigned char data = memory[startAddress + (i * blockSize) + j];
			if(data == 0) {
				printf(".");
			} else {
				printf("%c", data);
			}
		}
		printf("\n");
	}
}
void printStatus() {
	printf("\n---- System Status ----\n\n");
	printf("AF:  0x%04X\n", R.AF);
	printf("BC:  0x%04X\n", R.BC);
	printf("DE:  0x%04X\n", R.DE);
	printf("HL:  0x%04X\n", R.HL);
	printf("AF': 0x%04X\n", R.AFshadow);
	printf("BC': 0x%04X\n", R.BCshadow);
	printf("DE': 0x%04X\n", R.DEshadow);
	printf("HL': 0x%04X\n", R.HLshadow);
	printf("IX:  0x%04X\n", R.IX);
	printf("IY:  0x%04X\n", R.IY);
	printf("SP:  0x%04X\n", R.SP);
	printf("I:   0x%02X\n", R.I);
	printf("R:   0x%02X\n", R.R);
	printf("PC:  0x%04X\n", R.PC);
	printf("\n\nCurrent memory block:\n\n");
	printMemoryBlock(((R.PC) / (16*8))*16*8);
	printf("\n\nCurrent stack:\n\n");
	printMemoryBlock(((R.SP) / (16*8))*16*8);
	printf("\n---- ------------- ----\n");
}
int readROMFile() {
	char fileName[25] = "x.dat";
	FILE *filePointer;

	printf("Enter name of the binary to execute\n");
	//gets(fileName);

	printf("Reading file %s:\n", fileName);

	filePointer = fopen(fileName, "r");

	if(filePointer == NULL) {
		printf("Error opening file, check the spelling of the filename.\n");
		return -1;
	}
	//get the number of bytes for the file
	fseek(filePointer, 0L, SEEK_END);
	long numbytes = ftell(filePointer);
	printf("reading %Ld bytes\n", numbytes);

	//Goto start of file
	fseek(filePointer, 0L, SEEK_SET);

	char* buffer = (char*)calloc(numbytes, sizeof(char));

	if(buffer == NULL)
			return 1;

	fread(buffer, sizeof(char), numbytes, filePointer);

	strcpy(memory, buffer);

	fclose(filePointer);

	free(buffer);
}



//Instruction implementations

//For unimplemented instructions
int cpuInvalid() {
	printf("Unrecognized instruction (0x%X) at address 0x%04X.\n", memory[R.PC - 1] ,R.PC);
	return -1;
}

//0x00 : NOP
int cpuNop() {
	//Do nothing
	printf("NOP\n");
	return -1;
}

//0x01 LXI B,D16  (LD BC Val)
int cpuLDBCVal() {
	R.PC++;
	R.B = memory[R.PC];
	R.PC++;
	R.C = memory[R.PC];
	printf("LD_BC_VAL %02x %02x\n", R.B, R.C);
	return 0;
}

//0x02 STAX B  (LD BC A)
int cpuLDBCA() {
	memory[R.BC] = R.A;
	printf("LD_BC_A %02x %02x\n", R.BC, R.A);
	return 0;
}

//0x03 INX B  (INC BC)
int cpuIncBC() {
	printf("INC_BC\n");
	R.BC++;
	return 0;
}

//0x04 INR B (INC B)
int cpuIncB() {
	printf("INC_B\n");
	R.B++;
	return 0;
}

//0x05 DCR B (DEC B)
int cpuDecB() {
	printf("DEC_B\n");
	R.B--;
	return 0;
}

//0x11 LXI D,D16 (LD DE Val)
int cpuLDDEVal() {
	R.PC++;
	R.D = memory[R.PC];
	R.PC++;
	R.E = memory[R.PC];
	printf("LD_DE_VAL %02x %02x\n", R.D, R.E);
	return 0;
}

//0x12 STAX D  (LD DE A)
int cpuLDDEA() {
	memory[R.DE] = R.A;
	printf("LD_DE_A %02x %02x\n", R.DE, R.A);
	return 0;
}

//0x13 INX D  (INC DE)
int cpuIncDE() {
	printf("INC_DE\n");
	R.DE++;
	return 0;
}

//0x14 INR D (INC D)
int cpuIncD() {
	printf("INC_D\n");
	R.D++;
	return 0;
}


//0x15 DCR D (DEC D)
int cpuDecD() {
	printf("DEC_D\n");
	R.D--;
	return 0;
}

//0x21 LXI H,D16 (LD HL Val)
int cpuLDHLVal() {
	R.H = memory[R.PC];
	R.PC++;
	R.L = memory[R.PC];
	R.PC++;
	printf("LD_HL_VAL %02x %02x\n", R.H, R.L);
	return 0;
}

//0x31 LXI SP,D16 (LD SP Val)
int cpuLDSPVal() {
	R.S = memory[R.PC];
	R.PC++;
	R.P = memory[R.PC];
	R.PC++;
	printf("LD_SP_VAL %02x %02x\n", R.S, R.P);
	return 0;
}

//0x3C INR A (INC A)
int cpuIncA() {
	printf("INC_A\n");
	R.A++;
	return 0;
}

//0x3D DCR A (DEC A)
int cpuDecA() {
	printf("DEC_A\n");
	R.A--;
	return 0;
}

//0x3E MVI A,D8 (LD A)
int cpuLDA() {
	R.A = memory[R.PC];
	R.PC++;
	printf("LDA %02x\n", R.A);
	return 0;
}

//0xE1 POP H (POP HL)
int cpuPopHL() {
	printf("POP_HL\n");
	R.L = memory[R.SP];
	R.SP++;
	R.H = memory[R.SP];
	R.SP++;
	return 0;
}

//0xE5 PUSH H (PUSH HL)
int cpuPushHL() {
	printf("PUSH_HL\n");

	R.SP--;
	memory[R.SP] = R.H;
	R.SP--;

	memory[R.SP] = R.L;
	return 0;
}

//0xE9 PCHL (JP HL)
int cpuJPHL() {
	printf("JP_HL\n");

	R.PC = R.HL;
	return 0;
}

//Instruction function pointers

void setFunctionPointers() {
	for(int i = 0; i < 255; i++) {
		InstructionTable[i] = cpuInvalid;
	}
	InstructionTable[0x00] = cpuNop;
	InstructionTable[0x01] = cpuLDBCVal;
	InstructionTable[0x02] = cpuLDBCA;
	InstructionTable[0x03] = cpuIncBC;
	InstructionTable[0x04] = cpuIncB;
	InstructionTable[0x05] = cpuDecB;
	InstructionTable[0x11] = cpuLDDEVal;
	InstructionTable[0x12] = cpuLDDEA;
	InstructionTable[0x13] = cpuIncDE;
	InstructionTable[0x14] = cpuIncD;
	InstructionTable[0x15] = cpuDecD;
	InstructionTable[0x21] = cpuLDHLVal;
	InstructionTable[0x31] = cpuLDSPVal;
	InstructionTable[0x3C] = cpuIncA;
	InstructionTable[0x3D] = cpuDecA;
	InstructionTable[0x3E] = cpuLDA;
	InstructionTable[0xE1] = cpuPopHL;
	InstructionTable[0xE5] = cpuPushHL;
	InstructionTable[0xE9] = cpuJPHL;
}


/////////////////////////////////////////////////////////////////


int main() {
	reset();
	setFunctionPointers();

	memory[0x0000] = 0x21;
	memory[0x0001] = 0x42;
	memory[0x0002] = 0x42;
	memory[0x0003] = 0x3E;
	memory[0x0004] = 0xEA;
	memory[0x0005] = 0x3C;
	memory[0x0006] = 0x3C;
	memory[0x0007] = 0x3D;
	memory[0x0008] = 0xE9;


	memory[0x4242] = 0x21;
	memory[0x4243] = 0x98;
	memory[0x4244] = 0x76;
	memory[0x4245] = 0xE5;
	memory[0x4246] = 0xE1;

	printMemoryBlock(0);
	while(1) {
		int result = step();
		printStatus();
		if(result < 0) {
			break;
		}
	}
	return 0;
}
