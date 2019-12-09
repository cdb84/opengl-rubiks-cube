/*
 * triangle.c
 *
 *  Created on: Aug 28, 2017
 *      Author: Thumrongsak Kosiyatrakul
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
#define CUBE_VERTICES 36
#define CUBIES 27
#define VERTICES_SIZE CUBE_VERTICES*CUBIES
#define ROTATION_BOUND 90 * (M_PI/180)
#define GAP 0.01
#define SCRAMBLE_DEPTH 40


void keyboard(unsigned char, int, int);


vec4 colors[VERTICES_SIZE] ;

vec4 vertices[VERTICES_SIZE];
int num_vertices = VERTICES_SIZE;

GLuint model_view_ctm_location;
GLuint projection_ctm_location;
GLuint ctm_location;

mat4 model_view_ctm;
mat4 projection_ctm;

mat4 rotation_matrix;

mat4 origin_matrix;

vec4 eye_position = {0.0, 0.0, -0.25, 1.0};
vec4 at_vector = {0.0, 0.0, 0.0, 1.0};
vec4 up_vector = {0.0, 1.0, 0.0, 1.0};
float eye_degree = 0.05;

mat4 ctm_back, ctm_front, ctm_up, ctm_down, ctm_right, ctm_left;

mat4 cubies[CUBIES];


int range_back[9] = {0, CUBE_VERTICES*3, CUBE_VERTICES*6, CUBE_VERTICES*9, CUBE_VERTICES*12, CUBE_VERTICES*15, CUBE_VERTICES*18, CUBE_VERTICES*21, CUBE_VERTICES*24};
int range_front[9] = {CUBE_VERTICES*2, CUBE_VERTICES*5, CUBE_VERTICES*8, CUBE_VERTICES*11, CUBE_VERTICES*14, CUBE_VERTICES*17, CUBE_VERTICES*20, CUBE_VERTICES*23, CUBE_VERTICES*26};
int range_up[9] = {CUBE_VERTICES*6, CUBE_VERTICES*7, CUBE_VERTICES*8, CUBE_VERTICES*15, CUBE_VERTICES*16, CUBE_VERTICES*17, CUBE_VERTICES*24, CUBE_VERTICES*25, CUBE_VERTICES*26};
int range_down[9] = {0, CUBE_VERTICES, CUBE_VERTICES*2, CUBE_VERTICES*9, CUBE_VERTICES*10, CUBE_VERTICES*11, CUBE_VERTICES*18, CUBE_VERTICES*19, CUBE_VERTICES*20};
int range_right[9] = {CUBE_VERTICES*18, CUBE_VERTICES*19, CUBE_VERTICES*20, CUBE_VERTICES*21, CUBE_VERTICES*22, CUBE_VERTICES*23, CUBE_VERTICES*24, CUBE_VERTICES*25, CUBE_VERTICES*26};
int range_left[9] = {0, CUBE_VERTICES, CUBE_VERTICES*2, CUBE_VERTICES*3, CUBE_VERTICES*4, CUBE_VERTICES*5, CUBE_VERTICES*6, CUBE_VERTICES*7, CUBE_VERTICES*8};

void delay(int number_of_seconds) 
{ 
    int milli_seconds = 1000 * number_of_seconds; 
    clock_t start_time = clock(); 
    while (clock() < start_time + milli_seconds) 0; 
} 

void back()
{
	rotation_matrix = identity();
	ctm_back = matrix_multiply(rotation_z_matrix(90 * (M_PI/180)), ctm_back);
	r_string_back();
	
}

void front()
{
	rotation_matrix = identity();
	mat4 rotation = rotation_z_matrix(-ROTATION_BOUND);
	cubies[8] = matrix_multiply(rotation, cubies[8]);
	cubies[17] = matrix_multiply(rotation, cubies[17]);
	cubies[26] = matrix_multiply(rotation, cubies[26]);

	cubies[5] = matrix_multiply(rotation, cubies[5]);
	cubies[14] = matrix_multiply(rotation, cubies[14]);
	cubies[23] = matrix_multiply(rotation, cubies[23]);

	cubies[2] = matrix_multiply(rotation, cubies[2]);
	cubies[11] = matrix_multiply(rotation, cubies[11]);
	cubies[20] = matrix_multiply(rotation, cubies[20]);

	// ctm_front = matrix_multiply(rotation_z_matrix(-90 * (M_PI/180)), ctm_front);
	r_string_front();
}

void up()
{
	rotation_matrix = identity();
	ctm_up = matrix_multiply(rotation_y_matrix(ROTATION_BOUND), ctm_up);
	r_string_up();
}

void down()
{
	rotation_matrix = identity();
	ctm_down = matrix_multiply(rotation_y_matrix(ROTATION_BOUND), ctm_down);
	r_string_down();
}

void right()
{
	rotation_matrix = identity();
	ctm_right = matrix_multiply(rotation_x_matrix(ROTATION_BOUND), ctm_right);
	r_string_right();
}

void left()
{
	rotation_matrix = identity();
	ctm_left = matrix_multiply(rotation_x_matrix(ROTATION_BOUND), ctm_left);
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

		printf("%c%d ", pivot, times);

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
	for(int i = 0; i < CUBIES; i++)
	{
		cubies[i] = identity();
	}
	ctm_back = ctm_front = ctm_up = ctm_down = ctm_right = ctm_left = rotation_matrix = identity();
	origin_matrix = translate(-(CUBE_SIZE-GAP)/2, -(CUBE_SIZE-GAP)/2, -(CUBE_SIZE-GAP)/2);
	fill_colors(colors, VERTICES_SIZE);
	for(int i = 0; i < 27; i++){
		generate_cube(vertices, VERTICES_SIZE, CUBE_SIZE-GAP, i*36);
		uniform_transform(vertices, origin_matrix, i*36, i*36+36);
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
	projection_ctm = identity();
	model_view_ctm = look_at_vector(eye_position, at_vector, up_vector);
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
		glDrawArrays(GL_TRIANGLES, i*36, CUBE_VERTICES);
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
	puts(r_string);
}
void idle(void)
{
	for(int i = 0; i < CUBIES; i++)
	{
		cubies[i] = matrix_multiply(cubies[i], rotation_matrix);
	}
	// ctm_back = matrix_multiply(ctm_back, rotation_matrix);
	// ctm_front = matrix_multiply(ctm_front, rotation_matrix);
	// ctm_up = matrix_multiply(ctm_up, rotation_matrix);
	// ctm_down = matrix_multiply(ctm_down, rotation_matrix);
	// ctm_right = matrix_multiply(ctm_right, rotation_matrix);
	// ctm_left = matrix_multiply(ctm_left, rotation_matrix);
	
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	puts(r_string);
	arrays_init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Cubes with CTM");
	glewInit();
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}
