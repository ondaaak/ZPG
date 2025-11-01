#version 400

// --- KROK 1: Pøijetí promìnných z vertex shaderu (ØEŠÍ chyby) ---
// Tyto promìnné musíme deklarovat, aby se program správnì spojil (slinkoval).
in vec4 worldPos;
in vec3 worldNorm;

// --- KROK 2: Definice materiálu (Tohle jsi už mìl správnì) ---
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

out vec4 fragColor;

void main(void)
{
    // 1. Vezmeme barvu, kterou opravdu chceme
    vec3 color = material.diffuse;
    
    // 2. Falešnì "použijeme" VŠECHNY uniformy,
    //    aby je compiler nesmazal (optimalizoval pryè).
    color += material.ambient * 0.0;
    color += material.specular * 0.0;
    color += vec3(material.shininess * 0.0);

    // 3. Falešnì "použijeme" i 'in' promìnné
    color += worldPos.xyz * 0.0;
    color += worldNorm * 0.0;

    // 4. Výsledná barva je poøád jen ta z material.diffuse
    fragColor = vec4(color, 1.0);
}