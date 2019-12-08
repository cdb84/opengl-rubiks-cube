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

#include <stdio.h>
#include <math.h>

#define BUFFER_OFFSET(offset) ((GLvoid *)(offset))
#define CUBE_SIZE 0.15
#define CUBE_VERTICES 36
#define VERTICES_SIZE CUBE_VERTICES*27
#define ROTATION_BOUND 90 * (M_PI/180)
#define GAP 0.01

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
int range_back[9] = {0, CUBE_VERTICES*3, CUBE_VERTICES*6, CUBE_VERTICES*9, CUBE_VERTICES*12, CUBE_VERTICES*15, CUBE_VERTICES*18, CUBE_VERTICES*21, CUBE_VERTICES*24};
int range_front[9] = {CUBE_VERTICES*2, CUBE_VERTICES*5, CUBE_VERTICES*8, CUBE_VERTICES*11, CUBE_VERTICES*14, CUBE_VERTICES*17, CUBE_VERTICES*20, CUBE_VERTICES*23, CUBE_VERTICES*26};
int range_up[9] = {CUBE_VERTICES*6, CUBE_VERTICES*7, CUBE_VERTICES*8, CUBE_VERTICES*15, CUBE_VERTICES*16, CUBE_VERTICES*17, CUBE_VERTICES*24, CUBE_VERTICES*25, CUBE_VERTICES*26};
int range_down[9] = {0, CUBE_VERTICES, CUBE_VERTICES*2, CUBE_VERTICES*9, CUBE_VERTICES*10, CUBE_VERTICES*11, CUBE_VERTICES*18, CUBE_VERTICES*19, CUBE_VERTICES*20};
int range_right[9] = {CUBE_VERTICES*18, CUBE_VERTICES*19, CUBE_VERTICES*20, CUBE_VERTICES*21, CUBE_VERTICES*22, CUBE_VERTICES*23, CUBE_VERTICES*24, CUBE_VERTICES*25, CUBE_VERTICES*26};
int range_left[9] = {};

void back()
{
	rotation_matrix = identity();
	ctm_back = matrix_multiply(rotation_z_matrix(90 * (M_PI/180)), ctm_back);
	
}

void front()
{
	rotation_matrix = identity();
	ctm_front = matrix_multiply(rotation_z_matrix(-90 * (M_PI/180)), ctm_front);
}

void up()
{
	rotation_matrix = identity();
	ctm_up = matrix_multiply(rotation_y_matrix(ROTATION_BOUND), ctm_up);
}

void down()
{
	rotation_matrix = identity();
	ctm_down = matrix_multiply(rotation_y_matrix(ROTATION_BOUND), ctm_down);
}

void right()
{
	rotation_matrix = identity();
	ctm_right = matrix_multiply(rotation_x_matrix(ROTATION_BOUND), ctm_right);
}

void left()
{
	rotation_matrix = identity();
	ctm_left = matrix_multiply(rotation_x_matrix(ROTATION_BOUND), ctm_left);
}

void arrays_init(void)
{
	ctm_back = ctm_front = ctm_up = ctm_down = ctm_right = rotation_matrix = identity();
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

	// for multiple VAOs, use:
	// glBindVertexArray(vao[x]);
	// glDrawArrays(GL_TRIANGLES, start, end);
	// glBindVertexArray(vao[y]);
	// glDrawArrays(GL_TRIANGLES, start, end);

	// send a transformation matrix prior to rendering (will apply to all vertices proceeding)
	// 1: the location of the transformation matrix from the glGetUniformLocation() fn
	// 2: the number of matrices (1)
	// 3: transpose? (no)
	// 4: pointer to the matrix in application
	// the CTM "ctm" will essentially take on the uniform variable "ctm" in the vShader
	// glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	// draw the in-pipeline array of vertices, specific to the start and end index specified in 
	// the second and third argument
	// right now we will only draw the front, with it's respective CTM
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_back);
	for(int i = 0; i < 9; i++){
		glDrawArrays(GL_TRIANGLES, range_back[i], CUBE_VERTICES);
	}

	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_front);
	for(int i = 0; i < 9; i++){
		glDrawArrays(GL_TRIANGLES, range_front[i], CUBE_VERTICES);
	}

	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_up);
	for(int i = 0; i < 9; i++){
		glDrawArrays(GL_TRIANGLES, range_up[i], CUBE_VERTICES);
	}

	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_down);
	for(int i = 0; i < 9; i++){
		glDrawArrays(GL_TRIANGLES, range_down[i], CUBE_VERTICES);
	}

	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_right);
	for(int i = 0; i < 9; i++){
		glDrawArrays(GL_TRIANGLES, range_right[i], CUBE_VERTICES);
	}

	// glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
	printf("key: %i\n", key);
	if (key == 'q')
		exit(0);
	if (key == 'p')
		rotation_matrix = matrix_multiply(rotation_y_matrix(eye_degree), rotation_matrix);
		matrix_print(rotation_matrix);
	if (key == '[')
		rotation_matrix = identity();

	if (key == 'f') front();
	if (key == 'r') right();
	if (key == 'u') up();
	if (key == 'l');
	if (key == 'b') back();
	if (key == 'd') down();
	if (key == 's');
	//glutPostRedisplay();
}
void idle(void)
{
	ctm_back = matrix_multiply(ctm_back, rotation_matrix);
	ctm_front = matrix_multiply(ctm_front, rotation_matrix);
	ctm_up = matrix_multiply(ctm_up, rotation_matrix);
	ctm_down = matrix_multiply(ctm_down, rotation_matrix);
	ctm_right = matrix_multiply(ctm_right, rotation_matrix);
	
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
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
