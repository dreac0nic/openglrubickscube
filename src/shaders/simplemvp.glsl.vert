#version 430

layout (location = 0) in vec3 VertexPosition;

out vec3 color_position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	color_position = VertexPosition;
	
	gl_Position = projection*view*model*vec4(VertexPosition, 1.0f);
}
