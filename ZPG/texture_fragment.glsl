#version 400

in vec4 worldPos;
in vec3 worldNorm;
in vec2 texCoord; // Přijímáme UV souřadnice

uniform sampler2D textureUnitID; // Sampler pro texturu

// Přidáno, aby se předešlo chybě "Uniform not found"
// I když je nepoužíváme, musí tu být, protože DrawableObject je posílá
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
    // Jednoduše vezmeme barvu z textury
    fragColor = texture(textureUnitID, texCoord);
}