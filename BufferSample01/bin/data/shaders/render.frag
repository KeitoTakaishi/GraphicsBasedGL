#version 330 core
uniform sampler2D colorBuffer;
in vec2 TexCoord;
out vec4 c;

void main(){
  c = texture(colorBuffer, TexCoord);
  c.rgb = vec3(1.0) - c.rgb;
}
