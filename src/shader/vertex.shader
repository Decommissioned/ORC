#pragma include("master.shader")

uniform mat4 model_matrix;
uniform mat4 normal_matrix;

// IO variables section

in vec3 position;
in vec2 uv;
in vec3 normal;

out data
{
        vec3 position;
        vec2 uv;
        vec3 normal;

} output;

// Program logic section

void main()
{
        mat4 MVP = projection * view * model_matrix;
        gl_Position = MVP * vec4(position.xyz, 1.0);
               
        output.position = position;
        output.uv = uv;
        output.normal = normalize(mat3(normal_matrix) * normal);
}