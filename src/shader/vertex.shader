#pragma include("master.shader")

uniform mat4 model_matrix;
uniform mat4 normal_matrix;

in vec3 in_position;
in vec2 in_uv;
in vec3 in_normal;

out vec3 v_position;
out vec2 v_uv;
out vec3 v_normal;
out float v_dist;

void main()
{
        vec4 transformed_postion = model_matrix * vec4(in_position, 1.0);        
        vec4 clip_coordinate = projection * view * transformed_postion;        
        gl_Position = clip_coordinate;               
        v_position = transformed_postion.xyz;
        v_uv = in_uv;
        v_normal = mat3(normal_matrix) * in_normal;
        v_dist = clip_coordinate.w / render_distance;
}
