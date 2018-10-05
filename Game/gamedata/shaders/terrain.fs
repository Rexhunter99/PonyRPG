#version 400

uniform sampler2DArray diffusemap;
uniform sampler2DArray normalmap;

in	vec3 v_texcoord0;
in	vec3 v_texcoord1;

out	vec4 fragColor;

void main()
{
	vec4 diffuse = texture3D(diffusemap, v_texcoord0);
	vec4 normal = texture3D(normalmap, v_texcoord1);
	fragColor = diffuse;
}
