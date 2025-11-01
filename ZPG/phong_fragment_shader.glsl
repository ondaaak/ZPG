#version 400

#define MAX_LIGHTS 5

// P�ID�NY CHYB�J�C� TYPY, ABY ODPOV�DALY C++
#define TYPE_POINT 0
#define TYPE_SPOT  1
#define TYPE_DIRECTIONAL 2
#define TYPE_AMBIENT 3

struct lightSource {
    vec3 position;  // Pro Point a Spot
    vec3 diffuse;   // Pro v�echny
    vec3 specular;  // Pro Point, Spot, Directional
    vec3 direction; // Pro Spot a Directional
    // ZM�NA: Star� 'cutoff' nahrazen dv�ma nov�mi
    float innerCutoff; // Cosinus vnit�n�ho �hlu
    float outerCutoff; // Cosinus vn�j��ho �hlu
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
    // Toto bude barva z textury nebo materi�lu
    vec3 objectColor = vec3(0.0, 1.0, 0.0); 

    // Ambientn� sv�tlo se bude s��tat ze v�ech ambientn�ch sv�tel ve sc�n�
    vec3 ambientTotal = vec3(0.0); 
    vec3 diffuseTotal = vec3(0.0);
    vec3 specularTotal = vec3(0.0);

    for (int i = 0; i < numLights; i++) 
    {
        // Typ 3: Ambientn� sv�tlo
        // Jen p�i�te svou barvu k celkov�mu ambientn�mu osv�tlen� a kon��
        if (lights[i].type == TYPE_AMBIENT) {
            ambientTotal += lights[i].diffuse;
            continue;
        }

        vec3 lightDir;
        float attenuation = 1.0; // V�choz� (pro directional)
        float intensity = 1.0;   // V�choz� (pro point/directional)

        // ZCELA NOV� LOGIKA PRO ROZLI�EN� TYP� SV�TEL
        
        // Typ 0: Bodov� sv�tlo (Point Light)
        if (lights[i].type == TYPE_POINT) {
            lightDir = normalize(lights[i].position - worldPos.xyz);
            float distance = length(lights[i].position - worldPos.xyz);
            attenuation = 1.0 / (1.0 + 0.2 * distance + 0.3 * distance * distance);
        }
        
        // Typ 2: Sm�rov� sv�tlo (Directional Light)
        else if (lights[i].type == TYPE_DIRECTIONAL) {
            // Sm�rov� sv�tlo nem� pozici, jen sm�r
            // Pou�ijeme 'direction' z C++ a oto��me ho (lightDir je sm�r K sv�tlu)
            lightDir = normalize(-lights[i].direction); 
            attenuation = 1.0; // Sm�rov� sv�tlo nem� �tlum
        }
        
        // Typ 1: Ku�elov� sv�tlo (Spot Light)
        else if (lights[i].type == TYPE_SPOT) {
            lightDir = normalize(lights[i].position - worldPos.xyz);
            float distance = length(lights[i].position - worldPos.xyz);
            attenuation = 1.0 / (1.0 + 0.2 * distance + 0.3 * distance * distance);

            vec3 lightToFragDir = normalize(-lightDir);
            float theta = dot(lightToFragDir, normalize(lights[i].direction));

            // --- ZM�NA ZDE ---
            // M�sto 'step()', pou�ijeme logiku ze slajdu 13/26
            
            // Na�teme cosiny �hl�
            float alpha = lights[i].outerCutoff; // Vn�j�� okraj (nap�. cos(50) = 0.642)
            float beta = lights[i].innerCutoff;  // Vnit�n� okraj (nap�. cos(40) = 0.766)
            
            // Vzorec: (dotLF - alpha) / (beta - alpha)
            float intens = (theta - alpha) / (beta - alpha);
            
            // Omez�me v�sledek na rozsah <0, 1>
            intensity = clamp(intens, 0.0, 1.0); 
            // --- Konec zm�ny ---
        }

        // Pokud je sv�tlo mimo ku�el (intensity == 0.0), p�esko��me zbytek v�po�t�
        if (intensity == 0.0) continue;

        // Tento k�d je te� spole�n� pro Point, Spot i Directional
        // Pokud je povrch odvr�cen� od sv�tla, nep�isp�v�
        if (dot(norm, lightDir) < 0.0) {
            continue;
        }

        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        
        // Specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        // Aplikujeme �tlum (attenuation) a intenzitu ku�ele (intensity)
        diffuseTotal  += intensity * attenuation * diff * lights[i].diffuse;        
        specularTotal += intensity * attenuation * spec * lights[i].specular;
    }    

    // V�sledn� barva
    fragColor = vec4(ambientTotal * objectColor + diffuseTotal * objectColor + specularTotal * vec3(1.0), 1.0);
}