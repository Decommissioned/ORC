#version 400 core

layout(std140)
uniform global
{
	mat4 projection;
	mat4 view;
};

in vec2 uv;
in vec3 position;

out vec2 UV;

void main()
{
        gl_Position = vec4(position.xyz, 1.0);
		UV = uv;
}