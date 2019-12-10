#ifndef LIBMATRIX_H
#define LIBMATRIX_H
typedef struct
{
	float x;
	float y;
	float z;
	float w;
} vec4;

typedef struct
{
	vec4 a;
	vec4 b;
	vec4 c;
	vec4 d;
} mat4;

typedef struct
{
	float a, b, c, d, e, f, g, h, i;
} mat3;

typedef struct
{
	vec4 reflect_ambient, reflect_diffuse, reflect_specular;
	float shininess;
} material;

typedef struct 
{
	int a, b;
} vector;


#define RED {1.0, 0.0, 0.0, 1.0}
#define GREEN {0.0, 1.0, 0.0, 1.0}
#define BLUE {0.0, 0.0, 1.0, 1.0}
#define PINK {1.0, 0.0, 1.0, 1.0}
#define PURP {0.0, 1.0, 1.0, 1.0}
#define YELL {1.0, 1.0, 0.0, 1.0}
#define ORANGE {1.0, 165.0/256.0, 0.0, 1.0}


#define WHITE {1.0, 1.0, 1.0, 1.0}
#define BLACK {0.0, 0.0, 0.0, 1.0}

#define POOL_TABLE_UNIT 7.0

#define SPHERE_THETA 45

#define CUBE_SLIGHT 0.2

// order: 1, 2, 3, 1, 3, 4 for a square

const vec4 COLORS[72] =
    {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE,
     RED, RED, RED, RED, RED, RED,
     GREEN, GREEN, GREEN, GREEN, GREEN, GREEN,
     ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE,
     WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
     YELL, YELL, YELL, YELL, YELL, YELL,
		 BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
		 BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
		 BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
		 BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
		 BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
		 BLACK, BLACK, BLACK, BLACK, BLACK, BLACK
		 };

const mat4 ORTHO_CVV = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 1}};

#ifndef M_PI
	#define M_PI 3.141592
#endif

#include "libmatrix.c"

void assert_vector_equiv(vec4, vec4);
void assert_matrix_equiv(mat4, mat4);
void vector_print(vec4);
vec4 vector_scalar_multiply(float, vec4);
vec4 vector_add(vec4, vec4);
vec4 vector_subtract(vec4, vec4);
float vector_dot_product(vec4, vec4);
vec4 vector_cross_product(vec4, vec4);
float vector_magnitude(vec4);
vec4 vector_normalize(vec4);
void matrix_print(mat4);
mat4 matrix_scalar_multiply(float, mat4);
mat4 matrix_add(mat4, mat4);
mat4 matrix_subtract(mat4, mat4);
mat4 matrix_multiply(mat4, mat4);
mat4 matrix_transpose(mat4);
vec4 matrix_multiply_vector(mat4, vec4);
mat4 matrix_cofactor(mat4);
void print_sub_4(mat3);
float matrix3_determinant(mat3);
float m3d(mat3);
mat4 matrix_minor(mat4);
mat4 matrix_invert(mat4);
mat4 rotation_z_matrix(float);
mat4 rotation_x_matrix(float);
mat4 rotation_y_matrix(float);
mat4 scaling_matrix(float, float, float);
mat4 scaling_matrix_vector(vec4);
mat4 translate(float, float, float);
mat4 translate_vector(vec4);
void gen_cone(vec4*, int, int, int);
void generate_torus(vec4*, int, int);
void generate_cube(vec4*, int, float, int);
void generate_rectangle(vec4*, int, float, float, float);
mat4 look_at(float, float, float, float, float, float, float, float, float);
mat4 look_at_vector(vec4, vec4, vec4);
mat4 frustum(float, float, float, float, float, float);
void fill_colors(vec4*, int);

#endif