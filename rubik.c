/*
 * triangle.c
 *
 *  Created on: 9 DEC 2019
 *      Author: Connor Berry
 */

#ifdef __APPLE__ // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif // __APPLE__

#include "initShader.h"
#include "libmatrix.h"
#include "solve_rc.c"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define BUFFER_OFFSET(offset) ((GLvoid *)(offset))
#define CUBE_SIZE 0.15
#define CUBE_VERTICES 72
#define CUBIES 27
#define VERTICES_SIZE CUBE_VERTICES*CUBIES
#define DEGREE_DELTA 2
#define ROTATION_BOUND DEGREE_DELTA * (M_PI/180)
#define CALL_COUNT 90/DEGREE_DELTA
#define GAP 0.01
#define SCRAMBLE_DEPTH 40


void keyboard(unsigned char, int, int);
void display(void);


vec4 colors[VERTICES_SIZE];
vec4 vertices[VERTICES_SIZE];
int num_vertices = VERTICES_SIZE;
GLuint ctm_location;
mat4 rotation_matrix;
mat4 origin_matrix;

int animating = 0;
int calls = 0;
void (* ctm_update_fn)();

float eye_degree = 0.005;

mat4 cubies[CUBIES];

void delay(int number_of_seconds) 
{ 
	int milli_seconds = 1000 * number_of_seconds; 
	clock_t start_time = clock(); 
	while (clock() < start_time + milli_seconds) 0; 
} 

void nothing()
{}

void back_animation()
{
	mat4 rotation = rotation_z_matrix(-ROTATION_BOUND);

	for(int i = 0; i < CALL_COUNT; i++)
	{
		cubies[24] = matrix_multiply(rotation, cubies[24]);
		cubies[15] = matrix_multiply(rotation, cubies[15]);
		cubies[6] = matrix_multiply(rotation, cubies[6]);

		cubies[21] = matrix_multiply(rotation, cubies[21]);
		cubies[12] = matrix_multiply(rotation, cubies[12]);
		cubies[3] = matrix_multiply(rotation, cubies[3]);

		cubies[18] = matrix_multiply(rotation, cubies[18]);
		cubies[9] = matrix_multiply(rotation, cubies[9]);
		cubies[0] = matrix_multiply(rotation, cubies[0]);

		display();
	}
	
}

void back()
{
	puts("Back");
	rotation_matrix = identity();
	back_animation();

	mat4 s = cubies[18];
	cubies[18] = cubies[24];
	cubies[24] = cubies[6];
	cubies[6] = cubies[0];
	cubies[0] = s;

	s = cubies[3];
	cubies[3] = cubies[9];
	cubies[9] = cubies[21];
	cubies[21] = cubies[15];
	cubies[15] = s;
	
	r_string_back();
	
}

void front_animation()
{
	mat4 rotation = rotation_z_matrix(ROTATION_BOUND);

	for(int i = 0; i < CALL_COUNT; i++){
		cubies[8] = matrix_multiply(rotation, cubies[8]);
		cubies[17] = matrix_multiply(rotation, cubies[17]);
		cubies[26] = matrix_multiply(rotation, cubies[26]);

		cubies[5] = matrix_multiply(rotation, cubies[5]);
		cubies[14] = matrix_multiply(rotation, cubies[14]);
		cubies[23] = matrix_multiply(rotation, cubies[23]);

		cubies[2] = matrix_multiply(rotation, cubies[2]);
		cubies[11] = matrix_multiply(rotation, cubies[11]);
		cubies[20] = matrix_multiply(rotation, cubies[20]);

		display();
	}
}

void front()
{
	puts("Front");
	rotation_matrix = identity();
	front_animation();

	mat4 s = cubies[8];
	cubies[8] = cubies[26];
	cubies[26] = cubies[20];
	cubies[20] = cubies[2];
	cubies[2] = s;

	s = cubies[17];
	cubies[17] = cubies[23];
	cubies[23] = cubies[11];
	cubies[11] = cubies[5];
	cubies[5] = s;

	r_string_front();
}

void up_animation()
{
	mat4 rotation = rotation_y_matrix(ROTATION_BOUND);
	for(int i = 0; i < CALL_COUNT; i++)
	{
		cubies[6] = matrix_multiply(rotation, cubies[6]);
		cubies[15] = matrix_multiply(rotation, cubies[15]);
		cubies[24] = matrix_multiply(rotation, cubies[24]);

		cubies[7] = matrix_multiply(rotation, cubies[7]);
		cubies[16] = matrix_multiply(rotation, cubies[16]);
		cubies[25] = matrix_multiply(rotation, cubies[25]);

		cubies[8] = matrix_multiply(rotation, cubies[8]);
		cubies[17] = matrix_multiply(rotation, cubies[17]);
		cubies[26] = matrix_multiply(rotation, cubies[26]);

		display();
	}
}

void up()
{
	puts("Up");
	rotation_matrix = identity();
	up_animation();

	mat4 s = cubies[6];
	cubies[6] = cubies[24];
	cubies[24] = cubies[26];
	cubies[26] = cubies[8];
	cubies[8] = s;

	s = cubies[15];
	cubies[15] = cubies[25];
	cubies[25] = cubies[17];
	cubies[17] = cubies[7];
	cubies[7] = s;

	r_string_up();
}
void down_animation()
{
	mat4 rotation = rotation_y_matrix(-ROTATION_BOUND);

	for(int i = 0; i < CALL_COUNT; i++)
	{
		cubies[2] = matrix_multiply(rotation, cubies[2]);
		cubies[11] = matrix_multiply(rotation, cubies[11]);
		cubies[20] = matrix_multiply(rotation, cubies[20]);

		cubies[1] = matrix_multiply(rotation, cubies[1]);
		cubies[10] = matrix_multiply(rotation, cubies[10]);
		cubies[19] = matrix_multiply(rotation, cubies[19]);

		cubies[0] = matrix_multiply(rotation, cubies[0]);
		cubies[9] = matrix_multiply(rotation, cubies[9]);
		cubies[18] = matrix_multiply(rotation, cubies[18]);

		display();
	}

}

void down()
{
	puts("Down");
	rotation_matrix = identity();
	down_animation();

	mat4 s = cubies[2];
	cubies[2] = cubies[20];
	cubies[20] = cubies[18];
	cubies[18] = cubies[0];
	cubies[0] = s;

	s = cubies[11];
	cubies[11] = cubies[19];
	cubies[19] = cubies[9];
	cubies[9] = cubies[1];
	cubies[1] = s;

	r_string_down();
}
void right_animation()
{
	mat4 rotation = rotation_x_matrix(ROTATION_BOUND);

	for(int i = 0; i < CALL_COUNT; i++)
	{
		cubies[26] = matrix_multiply(rotation, cubies[26]);
		cubies[25] = matrix_multiply(rotation, cubies[25]);
		cubies[24] = matrix_multiply(rotation, cubies[24]);

		cubies[23] = matrix_multiply(rotation, cubies[23]);
		cubies[22] = matrix_multiply(rotation, cubies[22]);
		cubies[21] = matrix_multiply(rotation, cubies[21]);

		cubies[20] = matrix_multiply(rotation, cubies[20]);
		cubies[19] = matrix_multiply(rotation, cubies[19]);
		cubies[18] = matrix_multiply(rotation, cubies[18]);

		display();
	}
}

void right()
{
	puts("Right");
	rotation_matrix = identity();
	right_animation();

	mat4 s = cubies[26];
	cubies[26] = cubies[24];
	cubies[24] = cubies[18];
	cubies[18] = cubies[20];
	cubies[20] = s;

	s = cubies[25];
	cubies[25] = cubies[21];
	cubies[21] = cubies[19];
	cubies[19] = cubies[23];
	cubies[23] = s;

	r_string_right();
}
void left_animation()
{
	mat4 rotation = rotation_x_matrix(-ROTATION_BOUND);

	for(int i = 0; i < CALL_COUNT; i++)
	{
		cubies[6] = matrix_multiply(rotation, cubies[6]);
		cubies[7] = matrix_multiply(rotation, cubies[7]);
		cubies[8] = matrix_multiply(rotation, cubies[8]);

		cubies[3] = matrix_multiply(rotation, cubies[3]);
		cubies[4] = matrix_multiply(rotation, cubies[4]);
		cubies[5] = matrix_multiply(rotation, cubies[5]);

		cubies[0] = matrix_multiply(rotation, cubies[0]);
		cubies[1] = matrix_multiply(rotation, cubies[1]);
		cubies[2] = matrix_multiply(rotation, cubies[2]);

		display();
	}
}

void left()
{
	puts("Left");
	rotation_matrix = identity();
	left_animation();

	mat4 s = cubies[6];
	cubies[6] = cubies[8];
	cubies[8] = cubies[2];
	cubies[2] = cubies[0];
	cubies[0] = s;

	s = cubies[7];
	cubies[7] = cubies[5];
	cubies[5] = cubies[1];
	cubies[1] = cubies[3];
	cubies[3] = s;

	r_string_left();
}

void shuffle()
{
	srand(time(NULL));
	char s[SCRAMBLE_DEPTH];
	for(int i = 0; i < SCRAMBLE_DEPTH; i++)
	{
		char t;
		int c = rand() % 6;
		switch(c){
			case 0:
				t = 'U';
				break;
			case 1:
				t = 'D';
				break;
			case 2:
				t = 'L';
				break;
			case 3:
				t = 'R';
				break;
			case 4:
				t = 'B';
				break;
			case 5:
				t = 'F';
				break;
			default:
				t = 'U';
		}
		s[i] = t;
	}

	puts(s);
	for(int i = 0; i < SCRAMBLE_DEPTH; i++)
	{
		printf("%c ", s[i]);
		keyboard(s[i], 0, 0);
		delay(1);
	}
	puts("Done scrambling!");
}

void solve()
{
	puts("Solving");
	
	char* sol = solve_rc();
	puts(sol);
	void (*fn)();
	int times, index = 0;
	char pivot;

	int strl = strlen(sol);
	for(int i = 0; i < strl; i+=2)
	{
		pivot = sol[index];
		times = (int)(sol[index + 1] - '0');

		// printf("%c%d ", pivot, times);

		switch(pivot){
			case 'U':
				fn = up;
				break;
			case 'D':
				fn = down;
				break;
			case 'L':
				fn = left;
				break;
			case 'R':
				fn = right;
				break;
			case 'F':
				fn = front;
				break;
			case 'B':
				fn = back;
				break;
			default:
				fn = up;
		}

		for(int x = 0; x < times; x++)
		{
			fn();
		}

		index += 2;
	}
	puts("Done solving");
}

void arrays_init(void)
{
	ctm_update_fn = nothing;
	for(int i = 0; i < CUBIES; i++)
	{
		cubies[i] = identity();
	}
	rotation_matrix = identity();
	origin_matrix = translate(-(CUBE_SIZE-GAP)/2, -(CUBE_SIZE-GAP)/2, -(CUBE_SIZE-GAP)/2);
	fill_colors(colors, VERTICES_SIZE);
	for(int i = 0; i < CUBIES; i++){
		generate_cube(vertices, VERTICES_SIZE, CUBE_SIZE-GAP, i*CUBE_VERTICES);
		uniform_transform(vertices, origin_matrix, i*CUBE_VERTICES, i*CUBE_VERTICES+CUBE_VERTICES);
	}
	// bottom left row: 0 - CUBE_VERTICES * 3
	uniform_transform(vertices, translate(-CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE), 0, CUBE_VERTICES); // b & d
	uniform_transform(vertices, translate(-CUBE_SIZE, -CUBE_SIZE, 0.0), CUBE_VERTICES, CUBE_VERTICES*2); // d
	uniform_transform(vertices, translate(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE), CUBE_VERTICES*2, CUBE_VERTICES*3); // f & d

	// middle left row: CUBE_VERTICES*3 - CUBE_VERTICES*6
	uniform_transform(vertices, translate(-CUBE_SIZE, 0.0, -CUBE_SIZE), CUBE_VERTICES*3, CUBE_VERTICES*4); // b & l
	uniform_transform(vertices, translate(-CUBE_SIZE, 0.0, 0.0), CUBE_VERTICES*4, CUBE_VERTICES*5); // l
	uniform_transform(vertices, translate(-CUBE_SIZE, 0.0, CUBE_SIZE), CUBE_VERTICES*5, CUBE_VERTICES*6); // f & l

	// top left row: CUBE_VERTICES*6 - CUBE_VERTICES*9
	uniform_transform(vertices, translate(-CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE), CUBE_VERTICES*6, CUBE_VERTICES*7); // b & u
	uniform_transform(vertices, translate(-CUBE_SIZE, CUBE_SIZE, 0.0), CUBE_VERTICES*7, CUBE_VERTICES*8); // u & l
	uniform_transform(vertices, translate(-CUBE_SIZE, CUBE_SIZE, CUBE_SIZE), CUBE_VERTICES*8, CUBE_VERTICES*9); // f & u

	// bottom middle row: CUBE_VERTICES*9 - CUBE_VERTICES*12
	uniform_transform(vertices, translate(0.0, -CUBE_SIZE, -CUBE_SIZE), CUBE_VERTICES*9, CUBE_VERTICES*10); // d & b
	uniform_transform(vertices, translate(0.0, -CUBE_SIZE, 0.0), CUBE_VERTICES*10, CUBE_VERTICES*11); // d
	uniform_transform(vertices, translate(0.0, -CUBE_SIZE, CUBE_SIZE), CUBE_VERTICES*11, CUBE_VERTICES*12); // f && d

	// middle middle row: CUBE_VERTICES*12 - CUBE_VERTICES*15
	uniform_transform(vertices, translate(0.0, 0.0, -CUBE_SIZE), CUBE_VERTICES*12, CUBE_VERTICES*13);
	uniform_transform(vertices, translate(0.0, 0.0, 0.0), CUBE_VERTICES*13, CUBE_VERTICES*14);
	uniform_transform(vertices, translate(0.0, 0.0, CUBE_SIZE), CUBE_VERTICES*14, CUBE_VERTICES*15); // f

	// top middle row: CUBE_VERTICES*15 - CUBE_VERTICES*18
	uniform_transform(vertices, translate(0.0, CUBE_SIZE, -CUBE_SIZE), CUBE_VERTICES*15, CUBE_VERTICES*16);
	uniform_transform(vertices, translate(0.0, CUBE_SIZE, 0.0), CUBE_VERTICES*16, CUBE_VERTICES*17);
	uniform_transform(vertices, translate(0.0, CUBE_SIZE, CUBE_SIZE), CUBE_VERTICES*17, CUBE_VERTICES*18); // f

	// bottom right row: CUBE_VERTICES*18 - CUBE_VERTICES*21
	uniform_transform(vertices, translate(CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE), CUBE_VERTICES*18, CUBE_VERTICES*19);
	uniform_transform(vertices, translate(CUBE_SIZE, -CUBE_SIZE, 0.0), CUBE_VERTICES*19, CUBE_VERTICES*20);
	uniform_transform(vertices, translate(CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE), CUBE_VERTICES*20, CUBE_VERTICES*21); // f

	// middle right row: CUBE_VERTICES*21 - CUBE_VERTICES*24
	uniform_transform(vertices, translate(CUBE_SIZE, 0.0, -CUBE_SIZE), CUBE_VERTICES*21, CUBE_VERTICES*22);
	uniform_transform(vertices, translate(CUBE_SIZE, 0.0, 0.0), CUBE_VERTICES*22, CUBE_VERTICES*23);
	uniform_transform(vertices, translate(CUBE_SIZE, 0.0, CUBE_SIZE), CUBE_VERTICES*23, CUBE_VERTICES*24); // f

	// top right row: CUBE_VERTICES*24 - CUBE_VERTICES*27
	uniform_transform(vertices, translate(CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE), CUBE_VERTICES*24, CUBE_VERTICES*25);
	uniform_transform(vertices, translate(CUBE_SIZE, CUBE_SIZE, 0.0), CUBE_VERTICES*25, CUBE_VERTICES*26);
	uniform_transform(vertices, translate(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE), CUBE_VERTICES*26, CUBE_VERTICES*27); // f

}

void init(void)
{
	// initialize shader programs
	GLuint program = initShader("vshader_rubik.glsl", "fshader_rubik.glsl");
	glUseProgram(program);

	// vertex array object initialization and binds it for buffering and rendering
	// it is possible to have multiple of these arrays for different sets of vertices
	GLuint vao;
	// args: 1 array, by reference of vao
	glGenVertexArrays(1, &vao);
	// bind it to the pipeline
	glBindVertexArray(vao);

	// transfer vertex attributes into the graphics pipeline
	GLuint buffer;
	// initialize buffer (one buffer)
	glGenBuffers(1, &buffer);
	// bind buffer to pipeline
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// allocate the space for the buffer (size of both vertices and the colors)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
	// copies data from the vertices array into the buffer, starting from 0  to the end of vertices
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	// copies data from the colors array into the buffer, starting where vertices ended
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

	// vshader attributes initializations
	// locate the attribute named vPosition in the vertex shader program
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	// enable this attribute
	glEnableVertexAttribArray(vPosition);
	// assign a pointer to vPosition
	// 1: specifies the index of the generic vertex attribute to be modified
	// 2: specifies the number of components for this attribute (vec4)
	// 3: specifies the data type of each component
	// 4: specifies whether fixed-point data values should be normalized
	// 5: specifies the byte offset between consecutive vertex attributes
	// 6: specifies an offset of the first component of the first generic 
	// 	  attribute in the arrray/buffer
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// locate the attribute named vColor in the shader program
	GLuint vColor = glGetAttribLocation(program, "vColor");
	// enable this attribute
	glEnableVertexAttribArray(vColor);
	// see above explanation, except this time we specify that the offset in the buffer 
	// is after the block of position vertices
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vertices));

	// stores the location of one particular uniform variable in from the program
	// for use later in displaying and controlling the uniform variable
	ctm_location = glGetUniformLocation(program, "ctm");

	// enable hidden surface removal
	glEnable(GL_DEPTH_TEST);
	// set the clearing color to white
	glClearColor(1.0, 1.0, 1.0, 1.0);
	// set depth range from 1.0 to 0.0
	glDepthRange(1, 0);
}
void display(void)
{
	// clear color buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set polygon modes to fill triangles in from the front, and mesh lines in the back
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	for(int i = 0; i < CUBIES; i++)
	{
		glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&cubies[i]);
		glDrawArrays(GL_TRIANGLES, i*CUBE_VERTICES, CUBE_VERTICES);
	}
	
	// right now we will only draw the front, with it's respective CTM
	glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
	// printf("key: %i\n", key);
	if (key == 'q')
		exit(0);
	if (key == 'p')
		rotation_matrix = matrix_multiply(rotation_y_matrix(eye_degree), rotation_matrix);
	if (key == 'P')
		rotation_matrix = matrix_multiply(rotation_x_matrix(-eye_degree), rotation_matrix);
	if (key == '[')
		rotation_matrix = identity();

	if (key == 'f' || key == 'F') front();
	if (key == 'r' || key == 'R') right();
	if (key == 'u' || key == 'U') up();
	if (key == 'l' || key == 'L') left();
	if (key == 'b' || key == 'B') back();
	if (key == 'd' || key == 'D') down();
	if (key == 's') shuffle();
	if (key == 'S') solve();
}
void idle(void)
{
	for(int i = 0; i < CUBIES; i++)
	{
		cubies[i] = matrix_multiply(cubies[i], rotation_matrix);
	}
	
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	arrays_init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Rubik's Cube");
	glewInit();
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}
