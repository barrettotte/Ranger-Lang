#include "../common.h"

// Return character position of c in string s, -1 not found
int find(char *s, int c){
    char *p;
    p = strchr(s, c);
    return (p ? p - s: -1);
}
