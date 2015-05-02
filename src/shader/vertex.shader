#version 400 core

layout(std140)
uniform global
{
	mat4 projection;
	mat4 view;
};

in vec3 position;
in vec2 uv;

out vec2 UV;

void main()
{
        gl_Position = vec4(position.xy, 0.0, 1.0);
		UV = uv;
}