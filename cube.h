#ifndef CUBE_H
#define CUBE_H

enum { WHITE = 1, RED = 2, BLUE = 4, YELLOW = 8, ORANGE = 16, GREEN = 32};

typedef struct{
    char descriptor_string[49];
    char U[3][3];
    char D[3][3];
    char F[3][3];
    char B[3][3];
    char R[3][3];
    char L[3][3];
}rubik;

void generate_cubie(vec4*, int);
void generate_rubik(vec4*, int);

#include "solve_rc.h"
#include "libmatrix.h"
#include "cube.c"
#endif