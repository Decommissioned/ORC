#version 400 core

// Uniforms section

layout(std140)
uniform global
{
        mat4 projection;
        mat4 view;
        vec3 eye;     // Position of the camera
        vec3 sun;     // Directional light (i.e. sun's light rays)
        vec3 ambient; // Global ambient light
        float render_distance;
};

uniform sampler2D sampler;

// IO variables section

in data
{
        vec2 uv;
        vec3 normal;
	
} interpolated;

out vec4 result;

// Program logic section

// TODO: light calculation

void main()
{

        float lambert_reflection = dot(interpolated.normal, sun);

        result = clamp(lambert_reflection, 0.1, 1.0)  * texture(sampler, interpolated.uv);
}
