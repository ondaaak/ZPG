#version 400

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox; // Musí být samplerCUBE

void main()
{
    // Vzorkujeme texturu cubemapy pomocí 3D souøadnic
    FragColor = texture(skybox, TexCoords);
}