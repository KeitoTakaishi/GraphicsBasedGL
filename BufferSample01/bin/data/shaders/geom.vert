#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

in vec3 position;
in vec3 normal;
in vec4 color;

out vec4 Color;

void main(){
  gl_Position = proj * view * model * vec4(position, 1.0);
  Color = vec4(normalize(normal), 1.0);
}
