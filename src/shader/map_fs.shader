#pragma include("master.shader")

uniform sampler2D sampler;

in data
{
        
        vec2 uv;
        // vec3 normal;
        float distance;

} interpolated;

out vec4 result;

void main()
{
        
        vec4 tex = texture(sampler, interpolated.uv);
        result = mix(tex, sky_color, interpolated.distance);
        
}