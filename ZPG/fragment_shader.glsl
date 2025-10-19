#version 330
out vec4 frag_colour;
in vec3 vertexColor;
in vec4 worldPos;
void main () {
    frag_colour = vec4(worldPos.x, worldPos.y, worldPos.z, 1.0);
}