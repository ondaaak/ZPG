#version 400

#define MAX_LIGHTS 5
struct lightSource {
    vec3 position;
    vec3 diffuse;
    vec3 specular;
};

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
    vec3 objectColor = vec3(1.0, 1.0, 1.0);

    vec3 ambientTotal = vec3(0.05);
    vec3 diffuseTotal = vec3(0.0);
    vec3 specularTotal = vec3(0.0);

    for (int i = 0; i < numLights; i++) 
    {
        vec3 lightDir = normalize(lights[i].position - worldPos.xyz);

        
        if (dot(norm, lightDir) < 0.0 || dot(norm, viewDir) < 0.0) {
            continue;
        }

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        float distance = length(lights[i].position - worldPos.xyz);
        float attenuation = 1.0 / (1.0 + 0.3 * distance + 0.4 * distance * distance);

        diffuseTotal += attenuation * diff * lights[i].diffuse;        
        specularTotal += attenuation * spec * lights[i].specular;
    }    

    fragColor = vec4(ambientTotal * objectColor + diffuseTotal * objectColor + specularTotal * vec3(1.0), 1.0);
}
