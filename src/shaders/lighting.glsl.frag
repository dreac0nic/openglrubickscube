#version 430

in vec3 color;
in vec3 normal;
in vec3 light;

out vec4 gl_FragColor;

void main()
{
	float intensity = dot(light, normal);

	if(intensity > 0.95)
		intensity = 1.0;
	else if(intensity > 0.5)
		intensity = 0.5;
	else if(intensity > 0.25)
		intensity = 0.25;
	else
		intensity = 0.1f;

	gl_FragColor = vec4(intensity*color, 1.0);
}
