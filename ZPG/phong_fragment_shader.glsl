#version 400


#define MAX_LIGHTS 3
struct lightSource {
    vec3 position;
    vec3 diffuse;
    vec3 specular;
};


uniform lightSource lights[MAX_LIGHTS];


in vec4 worldPos;
in vec3 worldNorm;
out vec4 fragColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(void)
{


    vec3 norm = normalize(worldNorm);
    vec3 viewDir = normalize(viewPos - worldPos.xyz);
    vec3 objectColor = vec3(0.385, 0.647, 0.812);

    
    vec3 ambientTotal = vec3(0.1);
    vec3 diffuseTotal = vec3(0.0);
    vec3 specularTotal = vec3(0.0);
    
    for (int i = 0; i < 3; i++) 
    {
        vec3 lightDir = normalize(lights[i].position - worldPos.xyz);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   
        ambientTotal += 0.1 * lights[i].diffuse;          
        diffuseTotal += diff * lights[i].diffuse;        
        specularTotal += spec * lights[i].specular;

    }    



    

    fragColor = vec4(ambientTotal + diffuseTotal * objectColor + specularTotal * vec3(1.0), 1.0);
}
