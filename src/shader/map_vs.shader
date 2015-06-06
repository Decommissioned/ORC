#pragma include("master.shader")

in vec3 in_position;
in vec3 in_normal;

out vec2 v_uv;
out float v_dist;
out float v_height;

void main()
{
        vec4 clip_coordinate = projection * view * vec4(in_position, 1.0);
        gl_Position = clip_coordinate;
        v_uv = in_position.xz / 15.0;
        v_dist = clip_coordinate.w / render_distance;
        v_height = in_position.y;
}
