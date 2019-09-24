#version 150
precision mediump float;

uniform sampler2D colorBuffer;
//uniform sampler2DRect colorBuffer;
uniform sampler2D tex;
uniform vec3 lightPos;
uniform float far;
uniform float near;
uniform int depthMode;
uniform mat4 inv;

in vec4 vPosition;
in vec3 vNormal;
in vec4 vColor;
in vec4 vProjTexCoord;
in vec4 projectionTexCoord;
//in vec2 vTexCoord;
in vec4 vDepth;

out vec4 c;

float restDepth(vec4 _RGBA){
    float _rMask = 1.0;
    float _gMask = 1.0 / 255.0;
    float _bMask = 1.0 / (255.0 * 255.0);
    float _aMask = 1.0 / (255.0 * 255.0 * 255.0);
    float depth = dot(_RGBA, vec4(_rMask, _gMask, _bMask, _aMask));
return depth;
}

void main(){
    vec3  light = lightPos - vPosition.xyz;
    vec3  invLight  = normalize(inv * vec4(light, 0.0)).xyz;
    float diffuse = clamp(dot(vNormal, invLight), 0.2, 1.0);
    vec4 uv = vProjTexCoord; 
    float shadow = restDepth(texture2DProj(colorBuffer, uv));
    //float shadow = restDepth(textureProj(colorBuffer, uv));
    float depth = 1.0;
    vec4 depthColor = vec4(1.0, 1.0, 1.0,1.0);
    float linerDepth = 0.0;
    if(vDepth.w > 0.0){
       if(depthMode == 1){
            vec4 lightCoord = vDepth / vDepth.w;
            float clip = far - near;
            if( (vDepth.z/clip) - 0.0005 > shadow){
                depthColor  = vec4(0.5, 0.5, 0.5, 1.0);
            }
        }else{
            linerDepth = 1.0 / (far - near);
            vec3 v = vPosition.xyz;
            vec3 l = lightPos.xyz;
            linerDepth *= sqrt( pow(v.x-l.x, 2.0) + pow(v.y-l.y, 2.0) + pow(v.z-l.z, 2.0) );
            if(linerDepth  > shadow){
                depthColor  = vec4(0.2, 0.2, 0.2, 1.0);
            }
        }
    }
    
    
    //c = vec4(uv.x, uv.y, 0.0, 1.0);
    c = vec4(vec3(linerDepth), 1.0);    //this
    c = vec4(vec3(shadow), 1.0);  //sample
    //c = texture2DProj(colorBuffer, uv);
    c = vColor * vec4(vec3(diffuse), 1.0) * depthColor;
    //c = vec4(uv.x, uv.y, 0.0, 1.0);


    //vec2 _uv = vTexCoord;
    //c = texture(tex, vec2(_uv.x, _uv.y ));
}