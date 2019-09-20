#version 330 core
uniform mat4 v;
uniform mat4 p;
uniform mat4 m;
in vec4 position;

void main(){
    gl_Position = p * v * m * position;
}
