#version 400 core

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