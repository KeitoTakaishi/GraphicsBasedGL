#version 330 core

in vec4 position;
in vec2 texcoord;

out vec2 TexCoord;
void main(){
    TexCoord = texcoord;
    gl_Position = position;
}