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
        float distance;

} output;

// Program logic section

void main()
{
        vec4 transformed_postion = model_matrix * vec4(position, 1.0);
        
        vec4 clip_coordinate = projection * view * transformed_postion;
        
        gl_Position = clip_coordinate;
               
        output.position = transformed_postion.xyz;
        output.uv = uv;
        output.normal = mat3(normal_matrix) * normal;
        output.distance = clip_coordinate.w;
}