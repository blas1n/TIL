#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAFE_FREE(x) ((x) = (NULL)); (free(x));

#endif