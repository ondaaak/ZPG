#version 400

#define MAX_LIGHTS 5

// Typy sv�tel mus� odpov�dat C++ k�du
#define TYPE_POINT 0
#define TYPE_SPOT  1
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

    vec3 ambientTotal = vec3(0.0); 
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
        float intensity = 1.0;   

        if (lights[i].type == TYPE_POINT) {
            lightDir = normalize(lights[i].position - worldPos.xyz);
            float distance = length(lights[i].position - worldPos.xyz);
            attenuation = 1.0 / (1.0 + 0.2 * distance + 0.3 * distance * distance);
        }
        else if (lights[i].type == TYPE_DIRECTIONAL) {
            lightDir = normalize(-lights[i].direction); 
            attenuation = 1.0; 
        }
        else if (lights[i].type == TYPE_SPOT) {
            lightDir = normalize(lights[i].position - worldPos.xyz);
            float distance = length(lights[i].position - worldPos.xyz);
            attenuation = 1.0 / (1.0 + 0.2 * distance + 0.3 * distance * distance);
            vec3 lightToFragDir = normalize(-lightDir);
            float theta = dot(lightToFragDir, normalize(lights[i].direction));

            float alpha = lights[i].outerCutoff;
            float beta = lights[i].innerCutoff;
            float intens = (theta - alpha) / (beta - alpha);
            
            intensity = clamp(intens, 0.0, 1.0); 
        }

        if (intensity == 0.0) continue;
        if (dot(norm, lightDir) < 0.0) continue;

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); 

        diffuseTotal  += intensity * attenuation * diff * lights[i].diffuse;        
        specularTotal += intensity * attenuation * spec * lights[i].specular;
    }    

    // --- ZM�NA ZDE ---
    // Rozhodneme, jak� typ objektu kresl�me
    
    // P�edpokl�d�me, �e "sv�t�c�" materi�l m� difuzn� SLO�KU na nule
    // (Jako jsme nastavili v Application.cpp)
    if (material.diffuse == vec3(0.0))
    {
        // 1. SV�T�C� OBJEKT (Troj�heln�k, Sv�tlu�ka, Slunce)
        // Jen vykresl�me jeho "ambientn�" barvu, kter� te� slou�� jako emise.
        // Ignorujeme v�echna sv�tla ve sc�n�.
        fragColor = vec4(material.ambient, 1.0);
    }
    else
    {
        // 2. NORM�LN� OBJEKT (Strom, Ko�ka, Podlaha)
        // Spo��t�me norm�ln� Phong osv�tlen�.
        vec3 ambient = ambientTotal * material.ambient;
        vec3 diffuse = diffuseTotal * material.diffuse;
        vec3 specular = specularTotal * material.specular;
        fragColor = vec4(ambient + diffuse + specular, 1.0);
    }
}