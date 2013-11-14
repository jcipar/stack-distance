#include <cstdio>

#define test(x) \
    while(!(x)) { \
    fprintf(stderr, "%s:%d\n", __FILE__, __LINE__); \
    exit(1); \
    }
