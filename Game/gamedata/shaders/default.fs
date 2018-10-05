#version 400

uniform sampler2D diffuse;

in	vec2 vertUV;

out	vec4 fragColor;

void main()
{
	//fragColor = texture2D(diffuse, vertUV);
	fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
