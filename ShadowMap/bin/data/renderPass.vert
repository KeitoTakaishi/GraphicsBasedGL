#version 150
precision mediump float;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform mat4 tMatrix;
uniform mat4 vl;
uniform mat4 pl;
uniform mat4 tpv;
uniform mat4 scaleMat;


in vec4 position;
in vec3 normal;
in vec2 texcoord;
in vec4 color;

out vec4 vPosition;
out vec3 vNormal;
out vec4 vColor;
out vec4 vProjTexCoord;
out vec4 projectionTexCoord;
//out vec2 vTexCoord;
out vec4 vDepth;

void main(){
    vPosition = m * position;
    vNormal = normalize((m * vec4(normal,0.0)).xyz);
    vColor  = vec4(1.0, 1.0, 1.0, 1.0);
    
    //vProjTexCoord = scaleMat * tMatrix * pl * vl * m * position;
    //vProjTexCoord = tpv * m * position;
    vProjTexCoord = tpv * m * vec4(position.x, -1.0*position.y, 1.0*position.z, 1.0*position.w);
    
    
    vDepth  = pl * vl  * vPosition;
    //vTexCoord = texcoord;
    gl_Position = p * v * m * position;
}