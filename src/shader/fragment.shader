#pragma include("master.shader")

uniform sampler2D sampler;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

uniform float reflectivity;
uniform float roughness;

uniform vec3 sun;

// IO variables section

in data
{
        vec3 position;
        vec2 uv;
        vec3 normal;
	
} interpolated;

out vec4 result;

// Program logic section

void main()
{
       
        // Directional light source (sun)
        
        vec3  ambient_light = Ka * ambient;
        vec3  diffuse_light = Kd * dot(sun, interpolated.normal);
        
        float specular_factor = dot(reflect(normalize(-sun), interpolated.normal), normalize(eye - interpolated.position));
        specular_factor = max(specular_factor, 0.0);
        vec3 specular_light = Ks * reflectivity * specular_factor * pow(specular_factor, roughness);
        
        result = vec4(ambient_light + diffuse_light + specular_light , 1.0) * texture(sampler, interpolated.uv);
}
