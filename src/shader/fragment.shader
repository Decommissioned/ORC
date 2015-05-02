#version 400 core

in vec2 UV;

out vec4 output;

uniform sampler2D sampler;

void main()
{
	output = texture(sampler, UV);
}