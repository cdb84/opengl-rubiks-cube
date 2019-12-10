#include <math.h>
#include <stdio.h>
#include <assert.h>

float cot(float rad)
{
	return 1.0/tan(rad);
}
void assert_vector_equiv(vec4 a, vec4 b)
{
	assert(a.x == b.x);
	assert(a.y == b.y);
	assert(a.z == b.z);
	assert(a.w == b.w);
}

void assert_matrix_equiv(mat4 a, mat4 b)
{
	assert_vector_equiv(a.a, b.a);
	assert_vector_equiv(a.b, b.b);
	assert_vector_equiv(a.c, b.c);
	assert_vector_equiv(a.d, b.d);
	return;
}

void vector_print(vec4 a)
{
	printf("[ %.6f, %.6f, %.6f, %.6f ]\n", a.x, a.y, a.z, a.w);
}

vec4 vector_scalar_multiply(float s, vec4 a)
{
	vec4 res = {a.x * s, a.y * s, a.z * s, a.w * s};
	return res;
}

vec4 vector_add(vec4 a, vec4 b)
{
	vec4 res = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
	return res;
}

vec4 vector_subtract(vec4 a, vec4 b)
{
	vec4 res = {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
	return res;
}

float vector_dot_product(vec4 a, vec4 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

vec4 vector_cross_product(vec4 a, vec4 b)
{
	vec4 res = {(a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z),
							(a.x * b.y) - (a.y * b.x), 0.0};
	return res;
}

float vector_magnitude(vec4 v)
{
	return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2) + pow(v.w, 2));
}

vec4 vector_normalize(vec4 v)
{
	return vector_scalar_multiply(1 / vector_magnitude(v), v);
}

void matrix_print(mat4 m)
{
	printf("┌ %.6f\t%.6f\t%.6f\t%.6f\t┐\n"
				 "│ %.6f\t%.6f\t%.6f\t%.6f\t│\n"
				 "│ %.6f\t%.6f\t%.6f\t%.6f\t│\n"
				 "└ %.6f\t%.6f\t%.6f\t%.6f\t┘\n",
				 m.a.x, m.b.x, m.c.x, m.d.x,
				 m.a.y, m.b.y, m.c.y, m.d.y,
				 m.a.z, m.b.z, m.c.z, m.d.z,
				 m.a.w, m.b.w, m.c.w, m.d.w);
}

mat4 matrix_scalar_multiply(float a, mat4 m)
{
	vec4 x = vector_scalar_multiply(a, m.a);
	vec4 y = vector_scalar_multiply(a, m.b);
	vec4 z = vector_scalar_multiply(a, m.c);
	vec4 w = vector_scalar_multiply(a, m.d);
	mat4 res = {x, y, z, w};
	return res;
}

mat4 matrix_add(mat4 a, mat4 b)
{
	vec4 x = vector_add(a.a, b.a);
	vec4 y = vector_add(a.b, b.b);
	vec4 z = vector_add(a.c, b.c);
	vec4 w = vector_add(a.d, b.d);
	mat4 res = {x, y, z, w};
	return res;
}

mat4 matrix_subtract(mat4 a, mat4 b)
{
	vec4 x = vector_subtract(a.a, b.a);
	vec4 y = vector_subtract(a.b, b.b);
	vec4 z = vector_subtract(a.c, b.c);
	vec4 w = vector_subtract(a.d, b.d);
	mat4 res = {x, y, z, w};
	return res;
}

mat4 matrix_multiply(mat4 a, mat4 b)
{
	float c11 = (a.a.x * b.a.x) + (a.b.x * b.a.y) + (a.c.x * b.a.z) + (a.d.x * b.a.w);
	float c21 = (a.a.y * b.a.x) + (a.b.y * b.a.y) + (a.c.y * b.a.z) + (a.d.y * b.a.w);
	float c31 = (a.a.z * b.a.x) + (a.b.z * b.a.y) + (a.c.z * b.a.z) + (a.d.z * b.a.w);
	float c41 = (a.a.w * b.a.x) + (a.b.w * b.a.y) + (a.c.w * b.a.z) + (a.d.w * b.a.w);
	float c12 = (a.a.x * b.b.x) + (a.b.x * b.b.y) + (a.c.x * b.b.z) + (a.d.x * b.b.w);
	float c22 = (a.a.y * b.b.x) + (a.b.y * b.b.y) + (a.c.y * b.b.z) + (a.d.y * b.b.w);
	float c32 = (a.a.z * b.b.x) + (a.b.z * b.b.y) + (a.c.z * b.b.z) + (a.d.z * b.b.w);
	float c42 = (a.a.w * b.b.x) + (a.b.w * b.b.y) + (a.c.w * b.b.z) + (a.d.w * b.b.w);
	float c13 = (a.a.x * b.c.x) + (a.b.x * b.c.y) + (a.c.x * b.c.z) + (a.d.x * b.c.w);
	float c23 = (a.a.y * b.c.x) + (a.b.y * b.c.y) + (a.c.y * b.c.z) + (a.d.y * b.c.w);
	float c33 = (a.a.z * b.c.x) + (a.b.z * b.c.y) + (a.c.z * b.c.z) + (a.d.z * b.c.w);
	float c43 = (a.a.w * b.c.x) + (a.b.w * b.c.y) + (a.c.w * b.c.z) + (a.d.w * b.c.w);
	float c14 = (a.a.x * b.d.x) + (a.b.x * b.d.y) + (a.c.x * b.d.z) + (a.d.x * b.d.w);
	float c24 = (a.a.y * b.d.x) + (a.b.y * b.d.y) + (a.c.y * b.d.z) + (a.d.y * b.d.w);
	float c34 = (a.a.z * b.d.x) + (a.b.z * b.d.y) + (a.c.z * b.d.z) + (a.d.z * b.d.w);
	float c44 = (a.a.w * b.d.x) + (a.b.w * b.d.y) + (a.c.w * b.d.z) + (a.d.w * b.d.w);
	mat4 res =
			{{c11, c21, c31, c41},
			 {c12, c22, c32, c42},
			 {c13, c23, c33, c43},
			 {c14, c24, c34, c44}};
	return res;
}

mat4 matrix_transpose(mat4 m)
{
	mat4 res = {{m.a.x, m.b.x, m.c.x, m.d.x}, {m.a.y, m.b.y, m.c.y, m.d.y}, {m.a.z, m.b.z, m.c.z, m.d.z}, {m.a.w, m.b.w, m.c.w, m.d.w}};
	return res;
}

vec4 matrix_multiply_vector(mat4 m, vec4 v)
{
	vec4 col1 = vector_scalar_multiply(v.x, m.a);
	vec4 col2 = vector_scalar_multiply(v.y, m.b);
	vec4 col3 = vector_scalar_multiply(v.z, m.c);
	vec4 col4 = vector_scalar_multiply(v.w, m.d);

	vec4 res = vector_add(vector_add(vector_add(col1, col2), col3), col4);

	return res;
}

mat4 matrix_cofactor(mat4 m)
{
	mat4 cofactor = {{m.a.x, -m.a.y, m.a.z, -m.a.w},
									 {-m.b.x, m.b.y, -m.b.z, m.b.w},
									 {m.c.x, -m.c.y, m.c.z, -m.c.w},
									 {-m.d.x, m.d.y, -m.d.z, m.d.w}};
	return cofactor;
}

void print_sub_3(mat3 m)
{
	printf("%.6f %.6f %.6f, %.6f %.6f %.6f, %.6f %.6f %.6f \n",
				 m.a, m.b, m.c, m.d, m.e, m.f, m.g, m.h, m.i);
}

float matrix3_determinant(mat3 m)
{
	return m.a * m.e * m.i + m.b * m.f * m.g + m.c * m.d * m.h - m.g * m.e * m.c -
				 m.h * m.f * m.a - m.i * m.d * m.b;
}

float m3d(mat3 m)
{
	return matrix3_determinant(m);
}

mat4 matrix_minor(mat4 m)
{
	mat3 m11 = {m.b.y, m.c.y, m.d.y, m.b.z, m.c.z, m.d.z, m.b.w, m.c.w, m.d.w};
	mat3 m12 = {m.a.y, m.c.y, m.d.y, m.a.z, m.c.z, m.d.z, m.a.w, m.c.w, m.d.w};
	mat3 m13 = {m.a.y, m.b.y, m.d.y, m.a.z, m.b.z, m.d.z, m.a.w, m.b.w, m.d.w};
	mat3 m14 = {m.a.y, m.b.y, m.c.y, m.a.z, m.b.z, m.c.z, m.a.w, m.b.w, m.c.w};
	mat3 m21 = {m.b.x, m.c.x, m.d.x, m.b.z, m.c.z, m.d.z, m.b.w, m.c.w, m.d.w};
	mat3 m22 = {m.a.x, m.c.x, m.d.x, m.a.z, m.c.z, m.d.z, m.a.w, m.c.w, m.d.w};
	mat3 m23 = {m.a.x, m.b.x, m.d.x, m.a.z, m.b.z, m.d.z, m.a.w, m.b.w, m.d.w};
	mat3 m24 = {m.a.x, m.b.x, m.c.x, m.a.z, m.b.z, m.c.z, m.a.w, m.b.w, m.c.w};
	mat3 m31 = {m.b.x, m.c.x, m.d.x, m.b.y, m.c.y, m.d.y, m.b.w, m.c.w, m.d.w};
	mat3 m32 = {m.a.x, m.c.x, m.d.x, m.a.y, m.c.y, m.d.y, m.a.w, m.c.w, m.d.w};
	mat3 m33 = {m.a.x, m.b.x, m.d.x, m.a.y, m.b.y, m.d.y, m.a.w, m.b.w, m.d.w};
	mat3 m34 = {m.a.x, m.b.x, m.c.x, m.a.y, m.b.y, m.c.y, m.a.w, m.b.w, m.c.w};
	mat3 m41 = {m.b.x, m.c.x, m.d.x, m.b.y, m.c.y, m.d.y, m.b.z, m.c.z, m.d.z};
	mat3 m42 = {m.a.x, m.c.x, m.d.x, m.a.y, m.c.y, m.d.y, m.a.z, m.c.z, m.d.z};
	mat3 m43 = {m.a.x, m.b.x, m.d.x, m.a.y, m.b.y, m.d.y, m.a.z, m.b.z, m.d.z};
	mat3 m44 = {m.a.x, m.b.x, m.c.x, m.a.y, m.b.y, m.c.y, m.a.z, m.b.z, m.c.z};

	mat4 minor =
			{{m3d(m11), m3d(m21), m3d(m31), m3d(m41)},
			 {m3d(m12), m3d(m22), m3d(m32), m3d(m42)},
			 {m3d(m13), m3d(m23), m3d(m33), m3d(m43)},
			 {m3d(m14), m3d(m24), m3d(m34), m3d(m44)}};
	return minor;
}

mat4 matrix_invert(mat4 m)
{
	mat4 minor = matrix_minor(m);
	mat4 cofactor = matrix_cofactor(minor);
	mat4 transpose = matrix_transpose(cofactor);
	float determinant =
			m.a.x * minor.a.x - m.b.x * minor.b.x + m.c.x * minor.c.x - m.d.x * minor.d.x;
	mat4 res = matrix_scalar_multiply(1 / determinant, transpose);
	return res;
}

mat4 rotation_z_matrix(float radians)
{
	mat4 res = {{cos(radians), sin(radians), 0, 0}, {-sin(radians), cos(radians), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
	return res;
}

mat4 rotation_x_matrix(float radians)
{
	mat4 res = {{1, 0, 0, 0}, {0, cos(radians), sin(radians), 0}, {0, -sin(radians), cos(radians), 0}, {0, 0, 0, 1}};
	return res;
}

mat4 rotation_y_matrix(float radians)
{
	mat4 res = {{cos(radians), 0, -sin(radians), 0}, {0, 1, 0, 0}, {sin(radians), 0, cos(radians), 0}, {0, 0, 0, 1}};
	return res;
}

mat4 scaling_matrix(float x, float y, float z)
{
	mat4 res = {{x, 0, 0, 0}, {0, y, 0, 0}, {0, 0, z, 0}, {0, 0, 0, 1}};
	return res;
}

mat4 scaling_matrix_vector(vec4 a)
{
	return scaling_matrix(a.x, a.y, a.z);
}

void gen_cone(vec4 *buf, int deg, int offset, int len)
{
	// printf("Starting gen_cone at %d until %d", offset, len);
	float r = 0.5;
	float theta_rad = deg * (M_PI / 180);
	mat4 rmat = rotation_y_matrix(theta_rad);
	vec4 origin = {0.0, 0.0, 0.0, 1.0};
	vec4 br = {r, 0.0, 0.0, 1.0};
	vec4 tr = {r * cos(theta_rad), 0.0, r * sin(theta_rad), 1.0};
	vec4 top = {0.0, r, 0.0, 1.0};
	// top right is the one that gets the rotation
	// br = tr;
	for (int i = offset; i < len;)
	{
		buf[i] = origin;
		i++;  
		buf[i] = tr;
		i++;  
		buf[i] = br;
		i++;  
		buf[i] = top;
		i++;  
		buf[i] = tr;
		i++;  
		buf[i] = br;
		i++; 
		br = tr;
		tr = matrix_multiply_vector(rmat, tr);
	}
}
void generate_torus(vec4 *buf, int len, int deg)
{
	//first start with a circle offset by the origin
	float r0 = 0.25;	// the torus will be of radius 0.5
	float r1 = 0.125; //0.1275; // the radius of the disk we will rotate about y
	float theta_rad = deg * (M_PI / 180);
	mat4 trl = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {r0, 0, 0, 1}};
	mat4 rmatz = rotation_z_matrix(theta_rad);
	mat4 rmaty = rotation_y_matrix(theta_rad);
	mat4 rmaty90 = rotation_y_matrix(M_PI / 2);
	mat4 rmatx90 = rotation_x_matrix(M_PI / 2);

	int sections = 360 / deg;
	// start with the parameters for the first mesh of the segment
	vec4 r = {r1, 0.0, 0.0, 1.0};
	vec4 d = {r1 * cos(theta_rad), r1 * sin(theta_rad), 0.0, 1.0};
	vec4 dz = matrix_multiply_vector(rmaty, d);
	vec4 rz = matrix_multiply_vector(rmaty, r);
	vector_print(r);
	vector_print(d);
	puts("");
	// 0 thru 36 of buf is the first section
	int i;
	for (i = 0; i < 216;)
	{
		// trl matrix will move the section into position along the radius
		buf[i] = matrix_multiply_vector(trl, dz);
		i++;
		buf[i] = matrix_multiply_vector(trl, rz);
		i++;
		buf[i] = matrix_multiply_vector(trl, r);
		i++;
		buf[i] = matrix_multiply_vector(trl, dz);
		i++;
		buf[i] = matrix_multiply_vector(trl, r);
		i++;
		buf[i] = matrix_multiply_vector(trl, d);
		i++;
		r = d;
		rz = dz;
		d = matrix_multiply_vector(rmatz, d);
		dz = matrix_multiply_vector(rmatz, dz);
	}
	rmaty = rotation_y_matrix(theta_rad / 2);
	for (int section = 1; section < 36; section++)
	{
		for (int j = 0; j < 216, i < len; j++)
		{
			buf[i] = matrix_multiply_vector(rmaty, buf[i - 216]);
			i++;
			buf[i] = matrix_multiply_vector(rmaty, buf[i - 216]);
			i++;
			buf[i] = matrix_multiply_vector(rmaty, buf[i - 216]);
			i++;
			buf[i] = matrix_multiply_vector(rmaty, buf[i - 216]);
			i++;
			buf[i] = matrix_multiply_vector(rmaty, buf[i - 216]);
			i++;
			buf[i] = matrix_multiply_vector(rmaty, buf[i - 216]);
			i++;
		}
	}
	for (int x = 0; x < len; x++)
	{
		buf[x] = matrix_multiply_vector(rmatx90, buf[x]);
	}
}
void generate_rectangle(vec4 *buf, int len, float l, float w, float h)
{
	vec4 rectangle_vertices[36] = {
			// front face of cube
			{0.0, 0.0, 0.0, 1.0},
			{0.0, h, 0.0, 1.0},
			{w, 0.0, 0.0, 1.0},
			{w, 0.0, 0.0, 1.0},
			{0.0, h, 0.0, 1.0},
			{w, h, 0.0, 1.0},

			//right side of cube
			{w, 0.0, 0.0, 1.0},
			{w, h, 0.0, 1.0},
			{w, 0.0, l, 1.0},
			{w, 0.0, l, 1.0},
			{w, h, 0.0, 1.0},
			{w, h, l, 1.0},

			//back side of cube
			{w, 0.0, l, 1.0},
			{w, h, l, 1.0},
			{0.0, 0.0, l, 1.0},
			{0.0, 0.0, l, 1.0},
			{w, h, l, 1.0},
			{0.0, h, l, 1.0},

			//left side of cube
			{0.0, 0.0, l, 1.0},
			{0.0, h, l, 1.0},
			{0.0, 0.0, 0.0, 1.0},
			{0.0, 0.0, 0.0, 1.0},
			{0.0, h, l, 1.0},
			{0.0, h, 0.0, 1.0},

			//top of cube
			{0.0, h, 0.0, 1.0},
			{0.0, h, l, 1.0},
			{w, h, 0.0, 1.0},
			{w, h, 0.0, 1.0},
			{0.0, h, l, 1.0},
			{w, h, l, 1.0},

			//bottom of cube
			{0.0, 0.0, 0.0, 1.0},
			{w, 0.0, 0.0, 1.0},
			{0.0, 0.0, l, 1.0},
			{0.0, 0.0, l, 1.0},
			{w, 0.0, 0.0, 1.0},
			{w, 0.0, l, 1.0},
	};
	for (int i = 0; i < len && i < 36; i++)
	{
		buf[i] = rectangle_vertices[i];
	}
}

void generate_cube(vec4 *buf, int len, float delta, int offset)
{
	vec4 cube_vertices[72] = {
			//front 
			{0.0, 0.0, 0.0, 1.0},
			{0.0, delta, 0.0, 1.0},
			{delta, 0.0, 0.0, 1.0},
			{delta, 0.0, 0.0, 1.0},
			{0.0, delta, 0.0, 1.0},
			{delta, delta, 0.0, 1.0},

			//right side of cube
			{delta, 0.0, 0.0, 1.0},
			{delta, delta, 0.0, 1.0},
			{delta, 0.0, delta, 1.0},
			{delta, 0.0, delta, 1.0},
			{delta, delta, 0.0, 1.0},
			{delta, delta, delta, 1.0},

			//back side of cube
			{delta, 0.0, delta, 1.0},
			{delta, delta, delta, 1.0},
			{0.0, 0.0, delta, 1.0},
			{0.0, 0.0, delta, 1.0},
			{delta, delta, delta, 1.0},
			{0.0, delta, delta, 1.0},

			//left side of cube
			{0.0, 0.0, delta, 1.0},
			{0.0, delta, delta, 1.0},
			{0.0, 0.0, 0.0, 1.0},
			{0.0, 0.0, 0.0, 1.0},
			{0.0, delta, delta, 1.0},
			{0.0, delta, 0.0, 1.0},

			//top of cube
			{0.0, delta, 0.0, 1.0},
			{0.0, delta, delta, 1.0},
			{delta, delta, 0.0, 1.0},
			{delta, delta, 0.0, 1.0},
			{0.0, delta, delta, 1.0},
			{delta, delta, delta, 1.0},
						
						//bottom of cube
			{0.0, 0.0, 0.0, 1.0},
			{delta, 0.0, 0.0, 1.0},
			{0.0, 0.0, delta, 1.0},
			{0.0, 0.0, delta, 1.0},
			{delta, 0.0, 0.0, 1.0},
			{delta, 0.0, delta, 1.0},

			// ********* BLACK VERTICES ****************
			// front face of cube
			{0.0, 0.0, 0.0, 1.0},
			{0.0, delta, 0.0, 1.0},
			{delta, 0.0, 0.0, 1.0},
			{delta, 0.0, 0.0, 1.0},
			{0.0, delta, 0.0, 1.0},
			{delta, delta, 0.0, 1.0},

			//right side of cube
			{delta, 0.0, 0.0, 1.0},
			{delta, delta, 0.0, 1.0},
			{delta, 0.0, delta, 1.0},
			{delta, 0.0, delta, 1.0},
			{delta, delta, 0.0, 1.0},
			{delta, delta, delta, 1.0},

			//back side of cube
			{delta, 0.0, delta, 1.0},
			{delta, delta, delta, 1.0},
			{0.0, 0.0, delta, 1.0},
			{0.0, 0.0, delta, 1.0},
			{delta, delta, delta, 1.0},
			{0.0, delta, delta, 1.0},

			//left side of cube
			{0.0, 0.0, delta, 1.0},
			{0.0, delta, delta, 1.0},
			{0.0, 0.0, 0.0, 1.0},
			{0.0, 0.0, 0.0, 1.0},
			{0.0, delta, delta, 1.0},
			{0.0, delta, 0.0, 1.0},

			//top of cube
			{0.0, delta, 0.0, 1.0},
			{0.0, delta, delta, 1.0},
			{delta, delta, 0.0, 1.0},
			{delta, delta, 0.0, 1.0},
			{0.0, delta, delta, 1.0},
			{delta, delta, delta, 1.0},
						
						//bottom of cube
			{0.0, 0.0, 0.0, 1.0},
			{delta, 0.0, 0.0, 1.0},
			{0.0, 0.0, delta, 1.0},
			{0.0, 0.0, delta, 1.0},
			{delta, 0.0, 0.0, 1.0},
			{delta, 0.0, delta, 1.0},
	};
	for (int i = offset, j = 0; i < len && j < 72; i++, j++)
	{
		buf[i] = cube_vertices[j];
	}
}

vec4 generate_panel(vec4* buf, int len, int offset, vec4 start)
{
	mat4 rotation_y = rotation_y_matrix( -(SPHERE_THETA * (M_PI/180)) );
	mat4 rotation_z = rotation_z_matrix( (SPHERE_THETA * (M_PI/180)) );
	// with the start vector, it will rotate it once around the x 
	// axis and once around the z axis, and then it will rotate that point 
	// it created via x by the z axis as well.
	vec4 origin = start;
	vec4 tl = matrix_multiply_vector(rotation_y, start);
	vec4 br = matrix_multiply_vector(rotation_z, start);
	vec4 tr = matrix_multiply_vector(rotation_z, tl);

	// now rip them into the buffer
	buf[offset] 	= origin;
	buf[offset + 1]	= tr;
	buf[offset + 2] = br;
	buf[offset + 3] = origin;
	buf[offset + 4] = tl;
	buf[offset + 5] = tr;

	vector_print(origin);
	vector_print(tr);
	vector_print(br);

	vector_print(origin);
	vector_print(tl);
	vector_print(tr);
}

// we will use degree ten
void generate_sphere_section(vec4* buf, int len, int offset)
{
	float r = 1.0;
	int max_points = 36;
	float theta = SPHERE_THETA * (M_PI/180);


}

// we should probably just keep it standardized to 10
void generate_sphere(vec4 *buf, int len, float phi)
{
	// use z-axis rotation for these points
	float r = 1.0;
	int max_points = 360 / phi;
	// the amount to shrink each ring by is determined by the amount of points
	// divided by two and inverted, so that everything remains uniformly
	// proportional and eventually r will equal 0 for the last ring 
	float delta = 1/(max_points / 2);
	vec4 ring_a[max_points];
	vec4 ring_b[max_points];
	float theta = phi * (M_PI/180);
	mat4 rmat = rotation_z_matrix(theta);
	// rightmost point on the unit circle
	vec4 head = {r, 0.0, 0.0, 1.0};
	ring_a[0] = head; 
	// we will, for every ring of points, only generate 360/phi points
	for(int i = 1; i < max_points; i++)
	{
		// you fill the ring by rotation the preceeding point around the z axis
		ring_a[i] = matrix_multiply_vector(rmat, ring_a[i-1]);
	}
	r = r - delta;
	for(int i = 0; i < max_points; i++)
	{
		// the second ring just appears as a smaller ring later on down the line
		ring_b[i] = vector_scalar_multiply(r, ring_a[i]);
	}
	// TODO finish this later
}

void generate_pool_table(vec4* buf, int len, int offset)
{
	vec4 bl, br, tr, tl;
	bl = (vec4){-POOL_TABLE_UNIT, -POOL_TABLE_UNIT, 0.0, 1.0};
	br = (vec4){POOL_TABLE_UNIT, -POOL_TABLE_UNIT, 0.0, 1.0};
	tr = (vec4){POOL_TABLE_UNIT, POOL_TABLE_UNIT, 0.0, 1.0};
	tl = (vec4){-POOL_TABLE_UNIT, POOL_TABLE_UNIT, 0.0, 1.0};
	buf[offset] 		= bl;
	buf[offset + 1] = br;
	buf[offset + 2] = tr;
	buf[offset + 3] = bl;
	buf[offset + 4] = tr;
	buf[offset + 5] = tl;
}

mat4 scale(float factor)
{
	mat4 result = {{factor, 0, 0, 0}, {0, factor, 0, 0}, {0, 0, factor, 0}, {0, 0, 0, 1}};
	return result;
}

mat4 translate(float x, float y, float z)
{
	mat4 result = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
	result.d.x = x;
	result.d.y = y;
	result.d.z = z;
	return result;
}

mat4 translate_vector(vec4 a)
{
	return translate(a.x, a.y, a.z);
}

mat4 identity()
{
	mat4 result = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
	return result;
}

mat4 look_at(float eyex, float eyey, float eyez, float atx, float aty, float atz, float upx, float upy, float upz)
{
	vec4 e = {eyex, eyey, eyez, 1.0}; // looking at this object (vrp)
	vec4 a = {atx, aty, atz, 1.0};		// from this point
	vec4 vup = {upx, upy, upz, 0.0};	// this is the vector from the top of our heads, up

	vec4 vpn = vector_subtract(a, e);
	vec4 n = vector_normalize(vpn);

	vec4 u = vector_cross_product(vup, n);
	u = vector_normalize(u);

	vec4 v = vector_cross_product(n, u);
	v = vector_normalize(v);

	mat4 T = translate(-eyex, -eyey, -eyez);
	mat4 A = {u, v, n, {0, 0, 0, 1}};
	mat4 R = matrix_invert(A);
	mat4 V = matrix_multiply(R, T);
	return V;
}

mat4 look_at_vector(vec4 eye, vec4 at, vec4 up)
{
	return look_at(eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);
}
/* Frustum using Tan's method */
mat4 frustum(float left, float right,
			 float bottom, float top,
			 float near, float far)
{
	float a = (-2.0 * near) / (right - left);
	float b = (-2.0 * near) / (top - bottom);
	float A = (left + right) / (right - left);
	float B = (bottom + top) / (top - bottom);
	float C = (near + far) / (far - near);
	float D = -(2 * near * far) / (far - near);

	vec4 cola = {a, 0, 0, 0};
	vec4 colb = {0, b, 0, 0};
	vec4 colc = {A, B, C, -1};
	vec4 cold = {0, 0, D, 0};

	mat4 result = {cola, colb, colc, cold};
	// matrix_print(result);
	return result;
}
/* Frustum using OpenGL specifications */
mat4 frustum_gl(float left, float right,
				float bottom, float top,
				float near, float far)
{
	float a = (2*near)/(right - left);
	float b = (2*near)/(top - bottom);
	float A = (right + left)/(right - left);
	float B = (top + bottom)/(top - bottom);
	float C = -(far + near)/(far - near);
	float D = -(2*far*near)/(far - near);

	vec4 cola = {a, 0, 0, 0};
	vec4 colb = {0, b, 0, 0};
	vec4 colc = {A, B, C, -1};
	vec4 cold = {0, 0, D, 0};
	return (mat4){cola, colb, colc, cold};
}
mat4 fov_perspective(float fovy, float aspect, float near, float far)
{
	vec4 a = {-cot(fovy/2.0)/aspect, 0, 0, 0};
	vec4 b = {0, -cot(fovy/2.0), 0, 0};
	vec4 c = {0, 0, (near+far)/(far-near), -1};
	vec4 d = {0, 0, -(2*near*far)/(far-near), 0};
	return (mat4){a, b, c, d};
}
mat4 perspective(float left, float right,
								 float bottom, float top,
								 float near, float far)
{
	mat4 result = {{-(near / right),0,0,0},
						{0, -(near / top), 0, 0},
						{0, 0, (near + far) / (far - near), -1},
						{0,0,-(2 * near * far) / (far - near), 0}};

	return result;
}

void fill_colors_until(vec4* buf, int len, int offset)
{
	for(int i = offset; i < len; i++)
	{
		buf[i] = COLORS[i % 72];
	}
}

void fill_colors(vec4 *buf, int len)
{
	for (int i = 0; i < len; i++)
	{
		buf[i] = COLORS[i % 72];
	}
}

void fill_colors_with(vec4* buf, vec4 color, int offset, int len)
{
	// printf("Starting fill_colors_with at %d until %d", offset, len);
	for(int i = offset; i < len; i++)
	{
		buf[i] = color;
	}
}

void uniform_transform(vec4* buf, mat4 ctm, int offset, int len)
{
	// printf("Starting uniform transform at %d until %d", offset, len);
	for(int i = offset; i < len; i++)
	{
		buf[i] = matrix_multiply_vector(ctm, buf[i]);
	}
}

vec4 vector_abs(vec4 a)
{
	return (vec4){abs(a.x), abs(a.y), abs(a.z), abs(a.w)};
}

vec4 vector_abs_normalize(vec4 v)
{
	return vector_scalar_multiply(1 / vector_magnitude(vector_abs(v)), v);
}


void point_subtractions(vec4* buf, vec4 p1, vec4 p2, vec4 p3)
{
	buf[0] = vector_subtract(p2, p1);
	buf[1] = vector_subtract(p3, p2);
}

vec4 normal(vec4 a, vec4 b)
{
	vec4 n = vector_cross_product(a, b);
	return vector_abs_normalize(n);
}

vec4 triangle_normal(vec4 p1, vec4 p2, vec4 p3)
{
	vec4 vectors[2];
	point_subtractions(vectors, p1, p2, p3);
	puts("*");
	vector_print(vectors[0]);
	vector_print(vectors[1]);
	return normal(vectors[0], vectors[1]);
}

vec4 return_normal_at(int index, vec4* buf, int len)
{
	if(!(index >= len || index+1 >= len || index+2 >= len)){ 
		return triangle_normal(buf[index], buf[index+1], buf[index+2]);
	}
}

void fill_with_normals(vec4* vertices, vec4* normals, int offset, int len)
{
	for(int i = offset; i < len; i+=3)
	{
		vec4 cur_normal = return_normal_at(i, vertices, len);
		normals[i] = cur_normal;
		normals[i+1] = cur_normal;
		normals[i+1] = cur_normal;
	}
}

void generate_rubiks_cube(vec4* vertices, int offset, int len)
{

}