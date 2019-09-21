#version 330 core

uniform sampler2D t;
uniform vec3 lightPos;
uniform mat4 invMatrix;
uniform float time;
in vec4 Position;
in vec3 Normal;
in vec4 TexCoord;
in vec2 vTexCoord;

out vec4 outputColor;

void main(){
    vec3 lightDir = lightPos - Position.xyz;
    vec3  invLight = normalize(invMatrix * vec4(lightDir, 0.0)).xyz;
    float diffuse = clamp(dot(Normal, invLight), 0.1, 1.0);

    vec4 uv = TexCoord;
    uv.y =  mod(uv.y + time*1000.0, 2000.0);
    vec4 c = texture2DProj(t, uv + vec4(100.0, 1.0, 0.0, 0.0));
    //c = texture(t, vTexCoord );
    
    //c.rgb = vec3(1.0) - c.rgb;
    //outputColor = vec4( (TexCoord.x+1.0)/2.0, (TexCoord.y+1.0)/2.0, 0.0, 1.0   );
    if(c.r > 0.8){
        c.rgb *= 0.2;
    }

    outputColor = vec4(c)*0.7 + diffuse;
} 