#include "numerus.h"
#include "numerus_test.h"

short _num_is_zero(char *roman);
#define SIGN(x)              (((x) > 0)  - ((x) < 0))
#define ABS(x)               (((x) < 0) ? -(x) : (x))