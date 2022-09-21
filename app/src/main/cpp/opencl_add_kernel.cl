#include "add.h"

__kernel void call_test(global float* A, const float b)
{
    int index = get_global_id(0);
    A[index] = add(A[index], b);
}