#pragma include("master.shader")

uniform sampler2D sampler;
uniform vec3 sun;

in data
{
        
        vec2 uv;
        // vec3 normal;
        float distance;
        float height;

} interpolated;

out vec4 result;

void main()
{

        vec3 normal = vec3(0.0, 1.0, 0.0);
        
        vec3 hack_light = vec3(3.1 + interpolated.height);
        vec3 diffuse_light = vec3(max(dot(sun, normal), 0.0));
        
        vec4 tex = vec4(hack_light + diffuse_light, 1.0) * texture(sampler, interpolated.uv);
        result = mix(tex, sky_color, interpolated.distance * 5.0);
        
}