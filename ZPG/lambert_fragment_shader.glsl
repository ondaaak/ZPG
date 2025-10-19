#version 400
in vec3 worldPos;
in vec3 worldNorm;
out vec4 fragColor;
uniform vec3 lightPos;
void main (void)
{
	vec3 lightDir = lightPos - worldPos;

	float diff = max(dot(normalize(lightDir), normalize(worldNorm)), 0.0);
	vec4 ambient = vec4 (0.1 ,0.1 ,0.1 ,1.0);
	vec4 objectColor = vec4 (0.385 ,0.647 ,0.812 ,1.0);
	fragColor = ambient + ( diff * objectColor );
}