#version 400

uniform mat4	mvp;

in	vec3 position;
in	vec2 texcoord;

out vec2 vertUV;

void main()
{
	vertUV = texcoord;
	gl_Position = mvp * vec4(position, 1.0);
}