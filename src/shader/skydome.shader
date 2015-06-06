#pragma include("master.shader")

#ifdef _VERTEX_SHADER_
in vec3 in_position;
in vec2 in_uv;

out vec2 v_uv;
out float v_y;

const vec3 j = vec3(0.0,1.0,0.0);
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
mat4 tran(vec3 pos)
{
	return mat4(1.0,   0.0,   0.0,   0.0,
                0.0,   1.0,   0.0,   0.0,
                0.0,   0.0,   1.0,   0.0,
                pos.x, pos.y, pos.z, 1.0);
}
void main()
{
        // Since the W component = 1.0 perspective divide will not affect the values
        vec4 cs = projection * view * tran(eye) * rot(j, timestamp * 0.03) * vec4(in_position, 1.0);
        gl_Position = cs.xyww;
        v_uv = in_uv;
        v_y = 1.0;
}
#endif

#ifdef _FRAGMENT_SHADER_
uniform sampler2D sampler;

in vec2 v_uv;
in float v_y;

void main()
{
        float k = smoothstep(0.0, 0.01, (v_y + 1.0) * 0.5);
        vec4 tex = texture(sampler, v_uv);
        gl_FragColor = mix(sky_color, tex, k);
}
#endif
