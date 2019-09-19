#version 330 core

uniform mat4 gm;
uniform mat4 gv;
uniform mat4 gp;

uniform mat4 v;
uniform mat4 p;
uniform mat4 m;

uniform mat4 mv;

uniform mat4 gmv;
uniform mat4 gmvp;

uniform mat4 view;
uniform mat4 modelViewProjectionMatrix;
in vec4 position;

void main(){
    gl_Position = p * v * m * position;
    gl_Position = gmvp * position;

    
    //gl_Position = gp * gv * gm * position;
    //gl_Position = p * view * m * position;
    //gl_Position = p * gmv * position;
} 