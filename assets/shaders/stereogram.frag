#version 430 core

in vec2 uv;
out vec4 colorFrag;

uniform sampler2D depthMap;
uniform sampler2D colorMap;
uniform sampler2D random;
uniform sampler2D uvMap;

layout (std430, binding = 0) coherent buffer Color {
  ivec4 color[];
};

layout (std430, binding = 1) coherent buffer Uv {
  vec2 UV[];
};

subroutine vec4 stereoPass();
subroutine uniform stereoPass computePass;

float sampleDepth(vec2 uv) {
    return pow(texture(depthMap, uv).r, 5.0);
}

int index(vec2 co) {
    int width = 1280-1;
    int height = 720-1;
    int res = int((co.y*height+co.x)*width);
    return (res >= 0 && res < 1280*720) ? res : 0;
}
 
subroutine (stereoPass) vec4 firstPass() {
    vec2 currCoord = uv;
    for(int i = 0; i < 32; i++) {
        float depth = sampleDepth(currCoord);
        float sep = 0.5*depth/(depth + 1.8);
        if(currCoord.x < 0.0)
           break;
        currCoord.x -= sep;       
    }
    currCoord.x = mod(currCoord.x, 1.0);
    vec3 col = texture(colorMap, currCoord).rgb;
    int i = index(currCoord);
    atomicAdd(color[i].r, int(col.r*255));
    atomicAdd(color[i].g, int(col.g*255));
    atomicAdd(color[i].b, int(col.b*255));
    atomicAdd(color[i].a, 1);
    UV[index(uv)] = currCoord;
    return vec4(0.0);
} 

subroutine (stereoPass) vec4 secondPass() {
    vec2 coord = UV[index(uv)];
    vec3 col = vec4(color[index(coord)]/(color[index(coord)].w*255.0)).rgb;
    return vec4(mix(col, texture(random, coord*8.0).rgb, 0.8), 1.0);
}

void main() {
    colorFrag = computePass();
}