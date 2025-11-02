#version 400

#define MAX_LIGHTS 5
#define TYPE_POINT 0
#define TYPE_DIRECTIONAL 2
#define TYPE_AMBIENT 3

struct lightSource {
    vec3 position;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
    float innerCutoff;
    float outerCutoff;
    int type;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;
uniform lightSource lights[MAX_LIGHTS];
uniform int numLights;

in vec4 worldPos;
in vec3 worldNorm;
out vec4 fragColor;
uniform vec3 viewPos;

void main(void)
{
    vec3 norm = normalize(worldNorm);
    vec3 viewDir = normalize(viewPos - worldPos.xyz);

    vec3 ambientTotal = vec3(0.1); 
    vec3 diffuseTotal = vec3(0.0);
    vec3 specularTotal = vec3(0.0);

    for (int i = 0; i < numLights; i++) 
    {
        if (lights[i].type == TYPE_AMBIENT) {
            ambientTotal += lights[i].diffuse;
            continue;
        }

        vec3 lightDir;
        float attenuation = 1.0;

        if (lights[i].type == TYPE_POINT) {
            lightDir = normalize(lights[i].position - worldPos.xyz);
            float distance = length(lights[i].position - worldPos.xyz);
            attenuation = 1.0 / (1.0 + 0.2 * distance + 0.3 * distance * distance);
        }
        else if (lights[i].type == TYPE_DIRECTIONAL) {
            lightDir = normalize(-lights[i].direction); 
            attenuation = 1.0;
        }

        if (dot(norm, lightDir) < 0.0) continue;

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        diffuseTotal  += attenuation * diff * lights[i].diffuse;        
        specularTotal += attenuation * spec * lights[i].specular;
    }    

    vec3 ambient = ambientTotal * material.ambient;
    vec3 diffuse = diffuseTotal * material.diffuse;
    vec3 specular = specularTotal * material.specular;
    fragColor = vec4(ambient + diffuse + specular, 1.0);
}
