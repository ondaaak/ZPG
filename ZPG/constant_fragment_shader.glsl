#version 400

// --- KROK 1: P�ijet� prom�nn�ch z vertex shaderu (�E�� chyby) ---
// Tyto prom�nn� mus�me deklarovat, aby se program spr�vn� spojil (slinkoval).
in vec4 worldPos;
in vec3 worldNorm;

// --- KROK 2: Definice materi�lu (Tohle jsi u� m�l spr�vn�) ---
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
    
    // 2. Fale�n� "pou�ijeme" V�ECHNY uniformy,
    //    aby je compiler nesmazal (optimalizoval pry�).
    color += material.ambient * 0.0;
    color += material.specular * 0.0;
    color += vec3(material.shininess * 0.0);

    // 3. Fale�n� "pou�ijeme" i 'in' prom�nn�
    color += worldPos.xyz * 0.0;
    color += worldNorm * 0.0;

    // 4. V�sledn� barva je po��d jen ta z material.diffuse
    fragColor = vec4(color, 1.0);
}