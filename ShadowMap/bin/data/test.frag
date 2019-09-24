#version 330 core

uniform sampler2D tex;
in vec2 TexCoord;
out vec4 c;

void main(){
    c = texture(tex, TexCoord);
}   