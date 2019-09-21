#version 330 core

uniform mat4 v;
uniform mat4 p;
uniform mat4 m;
uniform mat4 tMatrix; //texture
uniform mat4 tvMatrix; //texture view 
uniform mat4 tpMatrix; //texture proj
uniform mat4 tpvMatrix; //texture proj view



in vec4 position;
in vec3 normal;
in vec2 texcoord;

out vec4 Position;
out vec3 Normal;
out vec4 TexCoord;
out vec2 vTexCoord;

void main(){
    Position = m * position; //world
    //Normal = normal;
    Normal = normalize(m * vec4(normal, 0.0)).xyz;
    TexCoord = tMatrix * tpMatrix * tvMatrix * Position;
    TexCoord =  tpvMatrix * Position;
    //vTexCoord = texcoord;
    gl_Position = p * v * m * position;
} 