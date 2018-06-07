#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS

#define WIDTH 1024
#define HEIGHT 768
#define HEADER 54

char R[HEIGHT][WIDTH], G[HEIGHT][WIDTH], B[HEIGHT][WIDTH];

int main() {
	FILE* fp1;
	FILE* fp2;
	fp1 = fopen("C:\\Users\\blAs1N\\Desktop\\SNE.bmp", "rb");
	fp2 = fopen("C:\\Users\\blAs1N\\Desktop\\SNE2.bmp", "wb");
	int i, j;

	for (i = 0; i < HEADER; i++)
		putc(getc(fp1), fp2);
	for (i = 0; i< HEIGHT; i++)
		for (j = 0; j < WIDTH; j++) {
			B[i][j] = getc(fp1); G[i][j] = getc(fp1); R[i][j] = getc(fp1);
		}

	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++) {
			putc(B[i][j], fp2); putc(G[i][j], fp2); putc(R[i][j], fp2);
			//putc(10, fp2); putc(10, fp2); putc(10, fp2);
			//putc((B[i][j] + G[i][j] + R[i][j]) / 3, fp2);
			//putc((B[i][j] + G[i][j] + R[i][j]) / 3, fp2);
			//putc((B[i][j] + G[i][j] + R[i][j]) / 3, fp2);
		}

	fclose(fp1);
	fclose(fp2);

	return 0;
}