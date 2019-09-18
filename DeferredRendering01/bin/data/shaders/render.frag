#version 330 core

precision mediump float;

const int LIGHT_NUM = 40;

struct Light {
    vec3 position;
    vec3 color;
    vec3 specular;
};

uniform mat4 model;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform int renderMode;
uniform Light light[LIGHT_NUM];
uniform float disCoef;
uniform float lightAttenuation;
uniform vec3 viewPos;

in vec2 TexCoord;

out vec4 c;

void main(){
  vec4 position = texture(gPosition, TexCoord);
  vec4 normal = texture(gNormal, TexCoord);
  vec4 albedo = texture(gAlbedo, TexCoord);

  vec4 deferredColor = vec4(0.0);
  deferredColor += albedo;


  for(int i = 0; i < LIGHT_NUM; i++){
    vec3 lightPos = light[i].position;
    vec3 lightDir = lightPos - position.xyz;
    float len = length(lightDir) * disCoef;
    float attenuation = 1.0 / (lightAttenuation * pow(len, 2.0));
    lightDir = normalize(lightDir);
    float diffuse = max(0.0, dot(normal.xyz, lightDir));
    vec3 diffuseColor = max(0.0, dot(normal.xyz, lightDir)) * light[i].color;
    //vec3 diffuseColor = max(0.0, dot(normal.xyz, lightDir)) * light[i].color;

    //specular
    vec3 viewDir = normalize(viewPos - position.xyz);
    vec3 halfEyeDir = (viewDir + lightDir) / 2.0;
    float specular = pow(max(0.0, dot(normal.xyz, halfEyeDir)), 16.0);
    vec3 specularColor = specular * light[i].specular;
    //vec3 specularColor = specular * light[i].color;

    deferredColor.rgb += (diffuseColor) * attenuation;
    //deferredColor.rgb += (diffuseColor + specularColor) * attenuation;
  }

  if(renderMode == 0){
    c = deferredColor;
  }else if(renderMode == 1){
    c = position;
  }else if(renderMode == 2){
    c = normal;
  }else{
    c = albedo;
  }
}
