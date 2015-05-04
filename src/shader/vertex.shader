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

uniform mat4 model_matrix;
uniform mat4 normal_matrix;

// IO variables section

in vec3 position;
in vec2 uv;
in vec3 normal;

out data
{
        vec2 uv;
        vec3 normal;

} output;

// Program logic section

void main()
{
        mat4 MVP = projection * view * model_matrix;
        gl_Position = MVP * vec4(position.xyz, 1.0);
               
        output.uv = uv;
        output.normal = normalize(mat3(normal_matrix) * normal);
        // output.normal = mat3(transpose(inverse(view * model_matrix))) * normal;
}
