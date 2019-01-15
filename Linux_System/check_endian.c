#include <stdio.h>

#define BIG 0
#define LITTLE 1

int CheckEndian();

int main(int argc, char** argv) {
	int endian = CheckEndian();

	if (endian == LITTLE)
		puts("Little Endian");

	else
		puts("Big Endian");

	return 0;
}

int CheckEndian() {
	int i = 0x00000001;
	return ((char*)&i)[0] ? LITTLE : BIG;
}
