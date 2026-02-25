#include "IntArray.h"

// void multiplyByX(IntArray *p, int x)
// {
//     for (int i = 0; i < p->size; i++) {
//         p->elements[i] *= x;
//     }
// }

void multiplyByX(IntArray *p)
{
    for (int i = 0; i < p->size; i++) {
        p->elements[i] *= 61;
    }
}