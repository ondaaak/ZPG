#version 400
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
uniform mat4 modelMatrix;
uniform mat4 projectMatrix;
uniform mat4 viewMatrix;
out vec3 worldPos;
out vec3 worldNorm;

void main(void)
{
    worldPos = vec3(modelMatrix * vec4(vp, 1.0));
    worldNorm = mat3(transpose(inverse(modelMatrix))) * vn;
    gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);
}
