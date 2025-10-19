# version 400
layout ( location = 0) in vec3 vp; // vertex position
layout ( location = 1) in vec3 vn; // vertex normal
uniform mat4 modelMatrix;
uniform mat4 projectMatrix;
uniform mat4 viewMatrix;
out vec3 worldPos;
out vec3 worldNorm;

void main ( void )
{
	worldPos = vec3 ( modelMatrix * vec4 ( vp ,1.0));
	worldNorm = vn; // problem
	gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);
}