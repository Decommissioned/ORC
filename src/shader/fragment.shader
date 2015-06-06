#pragma include("master.shader")

uniform sampler2D sampler;
uniform vec3 sun;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;
in float v_dist;

out vec4 color;

void main()
{      
        // Directional light source (sun)  
        // Ambient light component
        vec3 ambient_light = Ka * ambient;
        // Diffuse light component
        float diffuse_factor = max(dot(sun, v_normal), 0.0);
        vec3 diffuse_light = Kd * diffuse_factor;
        // Specular light component
        vec3 sun_reflection = reflect(-sun, v_normal);
        float specular_factor = max(dot(sun_reflection, normalize(eye - v_position)), 0.0);
        vec3 specular_light = Ks * pow(specular_factor, light_damping);
        // Phong shading model
        vec3 phong = ambient_light + diffuse_light + specular_light;        
        // Texturing fragment and calculating fog intensity
        vec4 tex = vec4(phong, 1.0) * texture(sampler, v_uv);
        color = mix(tex, sky_color, v_dist * 5.0);        
}
