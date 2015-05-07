#pragma include("master.shader")

#ifdef _VERTEX_SHADER_

in vec3 position;
in vec2 uv;

out data
{
        vec2 uv;
        
} output;

mat4 rot(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

const vec3 j = vec3(0.0,1.0,0.0);

void main()
{
        // Since the W component = 1.0 perspective divide will not affect the values
        vec4 cs = projection * view * rot(j, timestamp * 0.03) * vec4(position * (render_distance * 0.9) + eye, 1.0);
        
        gl_Position = cs.xyzw;
        
        output.uv = uv;
}

#endif

#ifdef _FRAGMENT_SHADER_

uniform sampler2D sampler;

in data
{
        vec2 uv;
	
} interpolated;

out vec4 fragment;

void main()
{
        fragment = texture(sampler, interpolated.uv);
}

#endif
