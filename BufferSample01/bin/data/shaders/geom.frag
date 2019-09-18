#version 330 core
layout (location = 0) out vec4 colorTexture;

in vec4 Color;

void main(){
  colorTexture = Color;
}
