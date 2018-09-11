#include <stdio.h>
#include <stdlib.h>

#define MAX_MODE 9
#define SAFE_FREE(a) a = NULL; free(a);
#define DEV(x,y) ((x - y) / 2)
#define OPP_1(x, y) ((x <= y) ? (y - x) / 2 : ((x - y) / 2))
#define OPP_2(x, y) ((x <= y) ? (x) : ((x - (2 * (x - y)))))
#define OPP_3(x, y) ((x <= y) ? (x / 2) : ((y - (x / 2) - 1)))
#define OPP_4(x, y) ((x <= y) ? (y - x + 1) : (2 * (x - y) - (x - y - 1)))

int input(char* guide);
void square(int film, char* stars, char* blank);
void triangle_1(int film, char* stars, char* blank);
void triangle_2(int film, char* stars, char* blank);
void triangle_3(int film, char* stars, char* blank);
void triangle_4(int film, char* stars, char* blank);
void pyramid(int film, char* stars, char* blank);
void reverse(int film, char* stars, char* blank);
void diamond(int film, char* stars, char* blank);
void hourglass(int film, char* stars, char* blank);
char* assign(int film, char word);