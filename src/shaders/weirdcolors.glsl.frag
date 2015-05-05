#version 430

in vec3 color_position;

out vec4 gl_FragColor;

void main()
{
	gl_FragColor = vec4(color_position.x + 0.5, color_position.y + 0.5, color_position.z + 0.5, 1.0);
}
