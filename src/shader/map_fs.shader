#pragma include("master.shader")

uniform sampler2D sampler;
uniform vec3 sun;

in vec2 v_uv;
in float v_dist;
in float v_height;

out vec4 color;

void main()
{
        vec3 normal = vec3(0.0, 1.0, 0.0);
        vec3 hack_light = vec3(3.1 + v_height);
        vec3 diffuse_light = vec3(max(dot(sun, normal), 0.0));
        vec4 tex = vec4(hack_light + diffuse_light, 1.0) * texture(sampler, v_uv);
        color = mix(tex, sky_color, v_dist * 5.0);
}
