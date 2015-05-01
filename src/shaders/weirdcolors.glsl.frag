#version 430

in vec3 color_position;

out vec4 gl_FragColor;

void main()
{
	gl_FragColor = vec4(color_position.x, color_position.y, color_position.z, 1.0);
}
