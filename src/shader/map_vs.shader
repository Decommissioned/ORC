#pragma include("master.shader")

in vec3 position;
in vec3 normal;

out data
{
        vec2 uv;
        // vec3 normal;
        float distance;

} output;

void main()
{

        vec4 clip_coordinate = projection * view * vec4(position, 1.0);
        gl_Position = clip_coordinate;
        
        output.uv = position.xz / 30.0;
        // output.normal = normal;
        output.distance = clip_coordinate.w / render_distance;

}