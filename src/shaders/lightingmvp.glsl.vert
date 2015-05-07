#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;
layout (location = 2) in vec3 VertexNormal;

out vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	color = VertexColor;

	gl_Position = projection*view*model*vec4(VertexPosition, 1.0f);
}
