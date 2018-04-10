#include <stdio.h>
#include <stdlib.h>

#define MAX_MODE 9
#define SAFE_FREE(a) a = NULL; free(a);
#define DEV(i,j) ((i - j) / 2)
#define OPP_1(x, y) ((x <= y) ? (y - x) / 2 : ((x - y) / 2))
#define OPP_2(x, y) ((x <= y) ? (x) : ((x - (2 * (x - y)))))
#define OPP_3(x, y) ((x <= y) ? (x / 2) : ((y - (x / 2) - 1)))
#define OPP_4(x, y) ((x <= y) ? (y - x + 1) : (2 * (x - y) - (x - y - 1)))

int input(char* guide);
void square(int film);
void triangle_1(int film);
void triangle_2(int film);
void triangle_3(int film);
void triangle_4(int film);
void pyramid(int film);
void reverse(int film);
void diamond(int film);
void hourglass(int film);
char* assign(int film, char word);