#pragma include("master.shader")

uniform sampler2D sampler;

uniform vec3 K; // Ka, Kd, Ks
uniform float reflectivity;
uniform float roughness;


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

        float diffuse = dot(interpolated.normal, sun) * K.y;
        
        vec3 reflection = reflect(-sun, interpolated.normal);
        float specular = dot(reflection, normalize(eye - interpolated.position));
        specular = K.z * reflectivity * pow(clamp(specular, 0.0, 1.0), roughness);

        result = (clamp(diffuse, 0.0, 1.0 ) + vec4(specular)) * texture(sampler, interpolated.uv);
        result = clamp(result, vec4(ambient * K.x, 0.0), vec4(0.9));
}
