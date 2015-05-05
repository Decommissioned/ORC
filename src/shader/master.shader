#version 400 core

layout(std140)
uniform global
{
        mat4 projection;
        mat4 view;
        vec3 eye;     // Position of the camera
        vec3 ambient; // Global ambient light
        vec3 attenuation;
        float render_distance;
};

float calc_attenuation(float distance)
{
        // A x² + B x + C
        return attenuation.x * distance * distance + attenuation.y * distance + attenuation.z;
}