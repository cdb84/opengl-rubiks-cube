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

vec4 colors[VERTICES_SIZE] ;

vec4 vertices[VERTICES_SIZE];
int num_vertices = VERTICES_SIZE;

GLuint model_view_ctm_location;
GLuint projection_ctm_location;
GLuint ctm_location;

mat4 model_view_ctm;
mat4 projection_ctm;

mat4 origin_matrix;

vec4 eye_position = {0.0, 0.0, -0.25, 1.0};
vec4 at_vector = {0.0, 0.0, 0.0, 1.0};
vec4 up_vector = {0.0, 1.0, 0.0, 1.0};
float eye_degree = 0.0;

mat4 front_ctm;
int front_range[9]= {0, CUBE_VERTICES*3, CUBE_VERTICES*6, CUBE_VERTICES*9, CUBE_VERTICES*12, CUBE_VERTICES*15, CUBE_VERTICES*18, CUBE_VERTICES*21, CUBE_VERTICES*24};

void front()
{
	front_ctm = matrix_multiply(rotation_z_matrix(90 * (M_PI/180)), front_ctm);
}

void arrays_init(void)
{
	front_ctm = identity();
	origin_matrix = translate(-CUBE_SIZE/2, -CUBE_SIZE/2, -CUBE_SIZE/2);
	fill_colors(colors, VERTICES_SIZE);
	for(int i = 0; i < 27; i++){
		generate_cube(vertices, VERTICES_SIZE, CUBE_SIZE-0.01, i*36);
		uniform_transform(vertices, origin_matrix, i*36, i*36+36);
	}
	// bottom left row: 0 - CUBE_VERTICES * 3
	uniform_transform(vertices, translate(-CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE), 0, CUBE_VERTICES);
	uniform_transform(vertices, translate(-CUBE_SIZE, -CUBE_SIZE, 0.0), CUBE_VERTICES, CUBE_VERTICES*2);
	uniform_transform(vertices, translate(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE), CUBE_VERTICES*2, CUBE_VERTICES*3);

	// middle left row: CUBE_VERTICES*3 - CUBE_VERTICES*6
	uniform_transform(vertices, translate(-CUBE_SIZE, 0.0, -CUBE_SIZE), CUBE_VERTICES*3, CUBE_VERTICES*4);
	uniform_transform(vertices, translate(-CUBE_SIZE, 0.0, 0.0), CUBE_VERTICES*4, CUBE_VERTICES*5);
	uniform_transform(vertices, translate(-CUBE_SIZE, 0.0, CUBE_SIZE), CUBE_VERTICES*5, CUBE_VERTICES*6);

	// top left row: CUBE_VERTICES*6 - CUBE_VERTICES*9
	uniform_transform(vertices, translate(-CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE), CUBE_VERTICES*6, CUBE_VERTICES*7);
	uniform_transform(vertices, translate(-CUBE_SIZE, CUBE_SIZE, 0.0), CUBE_VERTICES*7, CUBE_VERTICES*8);
	uniform_transform(vertices, translate(-CUBE_SIZE, CUBE_SIZE, CUBE_SIZE), CUBE_VERTICES*8, CUBE_VERTICES*9);

	// bottom middle row: CUBE_VERTICES*9 - CUBE_VERTICES*12
	uniform_transform(vertices, translate(0.0, -CUBE_SIZE, -CUBE_SIZE), CUBE_VERTICES*9, CUBE_VERTICES*10);
	uniform_transform(vertices, translate(0.0, -CUBE_SIZE, 0.0), CUBE_VERTICES*10, CUBE_VERTICES*11);
	uniform_transform(vertices, translate(0.0, -CUBE_SIZE, CUBE_SIZE), CUBE_VERTICES*11, CUBE_VERTICES*12);

	// middle middle row: CUBE_VERTICES*12 - CUBE_VERTICES*15
	uniform_transform(vertices, translate(0.0, 0.0, -CUBE_SIZE), CUBE_VERTICES*12, CUBE_VERTICES*13);
	uniform_transform(vertices, translate(0.0, 0.0, 0.0), CUBE_VERTICES*13, CUBE_VERTICES*14);
	uniform_transform(vertices, translate(0.0, 0.0, CUBE_SIZE), CUBE_VERTICES*14, CUBE_VERTICES*15);

	// top middle row: CUBE_VERTICES*15 - CUBE_VERTICES*18
	uniform_transform(vertices, translate(0.0, CUBE_SIZE, -CUBE_SIZE), CUBE_VERTICES*15, CUBE_VERTICES*16);
	uniform_transform(vertices, translate(0.0, CUBE_SIZE, 0.0), CUBE_VERTICES*16, CUBE_VERTICES*17);
	uniform_transform(vertices, translate(0.0, CUBE_SIZE, CUBE_SIZE), CUBE_VERTICES*17, CUBE_VERTICES*18);

	// bottom right row: CUBE_VERTICES*18 - CUBE_VERTICES*21
	uniform_transform(vertices, translate(CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE), CUBE_VERTICES*18, CUBE_VERTICES*19);
	uniform_transform(vertices, translate(CUBE_SIZE, -CUBE_SIZE, 0.0), CUBE_VERTICES*19, CUBE_VERTICES*20);
	uniform_transform(vertices, translate(CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE), CUBE_VERTICES*20, CUBE_VERTICES*21);

	// middle right row: CUBE_VERTICES*21 - CUBE_VERTICES*24
	uniform_transform(vertices, translate(CUBE_SIZE, 0.0, -CUBE_SIZE), CUBE_VERTICES*21, CUBE_VERTICES*22);
	uniform_transform(vertices, translate(CUBE_SIZE, 0.0, 0.0), CUBE_VERTICES*22, CUBE_VERTICES*23);
	uniform_transform(vertices, translate(CUBE_SIZE, 0.0, CUBE_SIZE), CUBE_VERTICES*23, CUBE_VERTICES*24);

	// top right row: CUBE_VERTICES*24 - CUBE_VERTICES*27
	uniform_transform(vertices, translate(CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE), CUBE_VERTICES*24, CUBE_VERTICES*25);
	uniform_transform(vertices, translate(CUBE_SIZE, CUBE_SIZE, 0.0), CUBE_VERTICES*25, CUBE_VERTICES*26);
	uniform_transform(vertices, translate(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE), CUBE_VERTICES*26, CUBE_VERTICES*27);

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
	model_view_ctm_location = glGetUniformLocation(program, "model_view");
	projection_ctm_location = glGetUniformLocation(program, "projection"); 
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
	glUniformMatrix4fv(model_view_ctm_location, 1, GL_FALSE, (GLfloat *)&model_view_ctm);
	glUniformMatrix4fv(projection_ctm_location, 1, GL_FALSE, (GLfloat *)&projection_ctm);
	// glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	// draw the in-pipeline array of vertices, specific to the start and end index specified in 
	// the second and third argument
	// right now we will only draw the front, with it's respective CTM
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&front_ctm);
	for(int i = 0; i < 9; i++){
		printf("(%d)\n", front_range[i]);
		glDrawArrays(GL_TRIANGLES, front_range[i], CUBE_VERTICES);
	}

	// glDrawArrays(GL_TRIANGLES, 0, num_vertices);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
	printf("key: %i\n", key);
	if (key == 'q')
		exit(0);

	if (key == 'f') front();
	if (key == 'r');
	if (key == 'u');
	if (key == 'l');
	if (key == 'b');
	if (key == 'd');
	//glutPostRedisplay();
}
void idle(void)
{
	eye_position = matrix_multiply_vector(rotation_y_matrix(eye_degree), eye_position);
	model_view_ctm = look_at_vector(eye_position, at_vector, up_vector);

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
