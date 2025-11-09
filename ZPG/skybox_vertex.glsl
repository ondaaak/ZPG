#version 400

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projectMatrix; // <-- TADY BYLA CHYBA (pøejmenováno)
uniform mat4 viewMatrix;

void main()
{
    TexCoords = aPos;
    
    // Odstraníme posun (poslední sloupec) z view matice
    // Tím se skybox bude otáèet s kamerou, ale ne posouvat
    mat4 viewNoTranslation = mat4(mat3(viewMatrix)); 
    
    vec4 pos = projectMatrix * viewNoTranslation * vec4(aPos, 1.0);
    
    // Trik, který zajistí, že skybox je VŽDY v pozadí
    // Z-souøadnice bude po dìlení 1.0 (maximální hloubka)
    gl_Position = pos.xyww;
}