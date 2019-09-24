#version 150
precision mediump float;
//layout (location = 0) out vec4 colorBuffer;
uniform sampler2D texture;
uniform vec3 lightPos;
uniform float far;
uniform float near;
uniform int mode;
uniform int depthMode;
uniform mat4 inv;

in vec4 vPosition;
in vec4 worldPosition;
in vec3 vNormal;
in vec4 vColor;
in vec2 vTexCoord;

out vec4 worldLightPosition;
out vec4 outputColor;


vec4 convRGBA(float depth){
float r = depth;
float g = fract(r * 255.0);
float b = fract(g * 255.0);
float a = fract(b * 255.0);
float coef = 1.0 / 255.0;
r -= g * coef;
g -= b * coef;
b -= a * coef;
return vec4(r, g, b, a);
}

void main(){
    vec4 convColor;
   
    float linerDepth = 1.0 / (far - near);
    if(depthMode == 1){
        //gl_FragCoord.zベース
        //convColor = convRGBA(gl_FragCoord.z);
        convColor = convRGBA(gl_FragCoord.z / (far - near));
    }else{
        //距離ベース
        float linerDepth = length(vPosition) / (far - near);
        //linerDepth *= length(vPosition);
        
        convColor = convRGBA(linerDepth);
        convColor = vec4(vec3(linerDepth), 1.0);
    }
    outputColor = convColor;
}
