#version 400
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 vt; 

uniform mat4 modelMatrix;
uniform mat4 projectMatrix;
uniform mat4 viewMatrix;
uniform float w = 1.0;

out vec4 worldPos;
out vec3 worldNorm;
out vec2 texCoord; 

void main(void)
{
    worldPos = modelMatrix * vec4(vp * w, w);
    
    worldNorm = mat3(transpose(inverse(modelMatrix))) * vn;
    texCoord = vt;
    
    gl_Position = projectMatrix * viewMatrix * worldPos;
}