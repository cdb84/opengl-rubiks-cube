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
#define CUBE_SIZE 0.5
#define VERTICES_SIZE 36

const vec4 colors[VERTICES_SIZE] =
		{GREEN, GREEN, GREEN, GREEN, GREEN, GREEN,
		 BLUE, BLUE, BLUE, BLUE, BLUE, BLUE,
		 RED, RED, RED, RED, RED, RED,
		 PINK, PINK, PINK, PINK, PINK, PINK,
		 PURP, PURP, PURP, PURP, PURP, PURP,
		 YELL, YELL, YELL, YELL, YELL, YELL};

vec4 vertices[VERTICES_SIZE];
int num_vertices = VERTICES_SIZE;

GLuint model_view_ctm_location;
GLuint projection_ctm_location;

mat4 model_view_ctm;
mat4 projection_ctm;

vec4 eye_position = {0.0, 0.75, -0.75, 1.0};
float eye_degree = 0.005;

void arrays_init(void)
{
	mat4 test_frustum = frustum(-0.5, 0.5, -0.5, 0.5, -0.2, 6000.0);
	mat4 translation = identity();
	generate_cube(vertices, num_vertices, 0.75);

	for(int i = 0; i < num_vertices; i++){
		vertices[i] = matrix_multiply_vector(translation, vertices[i]);
		// vertices[i] = matrix_multiply_vector(test_frustum, vertices[i]);
		vector_print(vertices[i]);
	}
}

void init(void)
{
	// hermanault: near -.2, far -300, right .5, left-.5, top  .5, bottom  -.5
	model_view_ctm = projection_ctm = identity();
	// initialize shader programs
	GLuint program = initShader("vshader_lighting.glsl", "fshader_lighting.glsl");
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
	// draw the in-pipeline array of vertices, specific to the start and end index specified in 
	// the second and third argument
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
	printf("key: %i\n", key);
	if (key == 'q')
		exit(0);

	//glutPostRedisplay();
}
void idle(void)
{
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
