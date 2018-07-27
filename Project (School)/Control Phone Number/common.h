#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN (30)
#define InputFlush() (fgetc(stdin))
#define SAFE_FREE(x) ((x) = (NULL)); (free(x));

char* Input(const char*);

#endif