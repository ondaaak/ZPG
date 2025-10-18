#version 330
out vec4 frag_colour;
in vec3 vertexColor;
in vec4 position;
void main () {
    frag_colour = vec4(position.x, position.y, position.z, 1.0);
}