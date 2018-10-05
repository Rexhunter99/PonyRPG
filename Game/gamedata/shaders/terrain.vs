#version 400

uniform mat4	mvp;

in	vec3 position;
in	vec3 texcoord0;

out vec3 v_texcoord0;

void main()
{
	v_texcoord0 = texcoord;
	gl_Position = mvp * vec4(position, 1.0);
}