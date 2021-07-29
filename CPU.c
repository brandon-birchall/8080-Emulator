#include <stdio.h>
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
        unsigned char H;
        unsigned char L;
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
	unsigned short I;
	unsigned short R;
	unsigned short PC;


} extern registers;


unsigned char memory[1 << 15];
struct registers R;

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
	R.I = 0;
	R.R = 0;
	R.PC = 0;
}

int main() {
	reset();
	printf("Hello, World!\n");
	return 0;
}
