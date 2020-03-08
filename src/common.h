#ifndef __COMMON_H__
#define __COMMON_H__

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/utils.h"

enum {
    T_PLUS, T_MINUS, T_STAR, T_SLASH, T_INTLIT
};

struct token {
    int token;
    int intvalue;
};


#endif