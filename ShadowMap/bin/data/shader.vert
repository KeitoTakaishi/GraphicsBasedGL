#version 150


uniform mat4 m;
uniform mat4 vl;
uniform mat4 pl;
uniform mat4 pv; //projection * view (from light)

in vec4 position;
in vec3 normal;
in vec4 color;
in vec2 texcoord;

out vec4 vPosition;
out vec3 vNormal;
out vec4 vColor;
out vec2 vTexCoord;

void main(){
vNormal = normal;
vPosition = pl * vl * m * position;
//vPosition = pv * m * position;
vColor = vec4(1.0);
gl_Position = vPosition;
}
