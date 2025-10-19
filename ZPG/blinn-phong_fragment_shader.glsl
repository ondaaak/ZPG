#version 400
in vec4 worldPos;
in vec3 worldNorm;
out vec4 fragColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(void)
{

    vec3 norm = normalize(worldNorm);
    vec3 lightDir = normalize(lightPos - worldPos.xyz);
    vec3 viewDir = normalize(viewPos - worldPos.xyz);

    vec3 ambient = vec3(0.1);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfDir), 0.0), 32);

    vec3 objectColor = vec3(0.385, 0.647, 0.812);

    fragColor = vec4(ambient + diff * objectColor + spec * vec3(1.0), 1.0);
}
