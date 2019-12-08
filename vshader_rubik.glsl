#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 color;

uniform mat4 ctm;

uniform mat4 model_view;
uniform mat4 projection;

void main()
{
	color = vColor;
	gl_Position = ctm * (projection * model_view * vPosition / vPosition.w);
	gl_Position.w = 1.0;
}
