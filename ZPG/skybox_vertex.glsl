#version 400

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projectMatrix;
uniform mat4 viewMatrix;

void main()
{
    TexCoords = aPos;
    
    mat4 viewNoTranslation = mat4(mat3(viewMatrix)); 
    
    vec4 pos = projectMatrix * viewNoTranslation * vec4(aPos, 1.0);
    
    gl_Position = pos.xyww;
}