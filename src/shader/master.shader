#version 400 core

layout(std140)
uniform global
{
        mat4 projection;
        mat4 view;
        vec3 eye;     // Position of the camera
        vec3 ambient; // Global ambient light
        vec3 attenuation_factor;
        float light_damping;
        float render_distance;
};

float calc_attenuation(float distance)
{
        // A x² + B x + C
        return attenuation_factor.x * distance * distance + attenuation_factor.y * distance + attenuation_factor.z;
}