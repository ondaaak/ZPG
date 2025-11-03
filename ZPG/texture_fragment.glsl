#version 330
out vec4 FragColor;
uniform sampler2D textureUnitID;
in vec2 uv;
void main () {
     //FragColor = vec4(uv,1.0,1.0); //test na funkcnost uv souřadnic
     FragColor = texture(textureUnitID, uv);
}