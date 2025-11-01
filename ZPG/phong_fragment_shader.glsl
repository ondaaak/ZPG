#version 400

#define MAX_LIGHTS 5

// PØIDÁNY CHYBÌJÍCÍ TYPY, ABY ODPOVÍDALY C++
#define TYPE_POINT 0
#define TYPE_SPOT  1
#define TYPE_DIRECTIONAL 2
#define TYPE_AMBIENT 3

struct lightSource {
    vec3 position;  // Pro Point a Spot
    vec3 diffuse;   // Pro všechny
    vec3 specular;  // Pro Point, Spot, Directional
    vec3 direction; // Pro Spot a Directional
    // ZMÌNA: Starý 'cutoff' nahrazen dvìma novými
    float innerCutoff; // Cosinus vnitøního úhlu
    float outerCutoff; // Cosinus vnìjšího úhlu
    int type;       // 0=Point, 1=Spot, 2=Directional, 3=Ambient
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
    // Toto bude barva z textury nebo materiálu
    vec3 objectColor = vec3(0.0, 1.0, 0.0); 

    // Ambientní svìtlo se bude sèítat ze všech ambientních svìtel ve scénì
    vec3 ambientTotal = vec3(0.0); 
    vec3 diffuseTotal = vec3(0.0);
    vec3 specularTotal = vec3(0.0);

    for (int i = 0; i < numLights; i++) 
    {
        // Typ 3: Ambientní svìtlo
        // Jen pøiète svou barvu k celkovému ambientnímu osvìtlení a konèí
        if (lights[i].type == TYPE_AMBIENT) {
            ambientTotal += lights[i].diffuse;
            continue;
        }

        vec3 lightDir;
        float attenuation = 1.0; // Výchozí (pro directional)
        float intensity = 1.0;   // Výchozí (pro point/directional)

        // ZCELA NOVÁ LOGIKA PRO ROZLIŠENÍ TYPÙ SVÌTEL
        
        // Typ 0: Bodové svìtlo (Point Light)
        if (lights[i].type == TYPE_POINT) {
            lightDir = normalize(lights[i].position - worldPos.xyz);
            float distance = length(lights[i].position - worldPos.xyz);
            attenuation = 1.0 / (1.0 + 0.2 * distance + 0.3 * distance * distance);
        }
        
        // Typ 2: Smìrové svìtlo (Directional Light)
        else if (lights[i].type == TYPE_DIRECTIONAL) {
            // Smìrové svìtlo nemá pozici, jen smìr
            // Použijeme 'direction' z C++ a otoèíme ho (lightDir je smìr K svìtlu)
            lightDir = normalize(-lights[i].direction); 
            attenuation = 1.0; // Smìrové svìtlo nemá útlum
        }
        
        // Typ 1: Kuželové svìtlo (Spot Light)
        else if (lights[i].type == TYPE_SPOT) {
            lightDir = normalize(lights[i].position - worldPos.xyz);
            float distance = length(lights[i].position - worldPos.xyz);
            attenuation = 1.0 / (1.0 + 0.2 * distance + 0.3 * distance * distance);

            vec3 lightToFragDir = normalize(-lightDir);
            float theta = dot(lightToFragDir, normalize(lights[i].direction));

            // --- ZMÌNA ZDE ---
            // Místo 'step()', použijeme logiku ze slajdu 13/26
            
            // Naèteme cosiny úhlù
            float alpha = lights[i].outerCutoff; // Vnìjší okraj (napø. cos(50) = 0.642)
            float beta = lights[i].innerCutoff;  // Vnitøní okraj (napø. cos(40) = 0.766)
            
            // Vzorec: (dotLF - alpha) / (beta - alpha)
            float intens = (theta - alpha) / (beta - alpha);
            
            // Omezíme výsledek na rozsah <0, 1>
            intensity = clamp(intens, 0.0, 1.0); 
            // --- Konec zmìny ---
        }

        // Pokud je svìtlo mimo kužel (intensity == 0.0), pøeskoèíme zbytek výpoètù
        if (intensity == 0.0) continue;

        // Tento kód je teï spoleèný pro Point, Spot i Directional
        // Pokud je povrch odvrácený od svìtla, nepøispívá
        if (dot(norm, lightDir) < 0.0) {
            continue;
        }

        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        
        // Specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        // Aplikujeme útlum (attenuation) a intenzitu kužele (intensity)
        diffuseTotal  += intensity * attenuation * diff * lights[i].diffuse;        
        specularTotal += intensity * attenuation * spec * lights[i].specular;
    }    

    // Výsledná barva
    fragColor = vec4(ambientTotal * objectColor + diffuseTotal * objectColor + specularTotal * vec3(1.0), 1.0);
}