#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <demo_api.h>

int demo_api_add(int a, int b, int *out)
{
    if (out == NULL) {
        fprintf(stderr, "Out param is NULL\n");
        return DEMO_API_PARM;
    }
    *out = a + b;
    return DEMO_API_OK;
}

int demo_api_mult(int a, int b, int *out)
{
    if (out == NULL) {
        fprintf(stderr, "Out param is NULL\n");
        return DEMO_API_PARM;
    }
    *out = a * b;
    return DEMO_API_OK;
}

int demo_api_double_if_pos(int a, int *out)
{
    if (out == NULL) {
        fprintf(stderr, "Out param is NULL\n");
        return DEMO_API_PARM;
    }
    if (a < 0) {
        fprintf(stderr, "a is negative, cannot proceed\n");
        return DEMO_API_RANGE;
    }

    *out = 2 * a;
    return DEMO_API_OK;
}
