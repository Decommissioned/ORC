layout(std140)
uniform global
{
        mat4 projection;
        mat4 view;
        vec3 eye;     // Position of the camera
        vec3 ambient; // Global ambient light
        vec3 attenuation_factor;
        vec4 sky_color;
        float light_damping;
        float render_distance;
        float timestamp;
};

float calc_attenuation(float distance)
{
        // A x² + B x + C
        return attenuation_factor.x * distance * distance + attenuation_factor.y * distance + attenuation_factor.z;
}