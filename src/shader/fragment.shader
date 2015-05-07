#pragma include("master.shader")

uniform sampler2D sampler;
uniform vec3 sun;

// Material data
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

// IO variables section

in data
{
        vec3 position;
        vec2 uv;
        vec3 normal;
        float distance;
	
} interpolated;

out vec4 result;

// Program logic section

void main()
{
       
        // Directional light source (sun)
        
        vec3  ambient_light = Ka * ambient;
        
        float diffuse_factor = max(dot(sun, interpolated.normal), 0.0);
        vec3  diffuse_light = Kd * diffuse_factor;
        
        vec3 sun_reflection = reflect(-sun, interpolated.normal);
        float specular_factor = max(dot(sun_reflection, normalize(eye - interpolated.position)), 0.0);
        vec3 specular_light = Ks * pow(specular_factor, light_damping);
        
        vec3 phong = ambient_light + diffuse_light + specular_light;
        
        result = vec4(phong, 1.0) * texture(sampler, interpolated.uv);
        
}