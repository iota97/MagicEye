#version 430 core

in vec2 uv;
out vec4 colorFrag;

uniform sampler2D depthMap;
uniform sampler2D colorMap;
uniform sampler2D uvMap;

uniform int bufferWidth;
uniform int bufferHeight;
uniform float time;

uniform float depthStrength;

layout (std430, binding = 0) coherent buffer Color {
  ivec4 color[];
};

layout (std430, binding = 1) coherent buffer Uv {
  vec2 UV[];
};

subroutine vec4 stereoPass();
subroutine uniform stereoPass computePass;

int index(vec2 co) {
    int width = bufferWidth-1;
    int height = bufferHeight-1;
    int res = int((co.y*height+co.x)*width);
    return (res >= 0 && res < bufferWidth*bufferHeight) ? res : 0;
}

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}
 
float sampleDepth(vec2 uv) {
    return pow(texture(depthMap, uv).r, depthStrength);
}

bool checkEdge(vec2 co) {
    vec2 texelSize = 2.0/textureSize(colorMap, 0);
    co -= mod(co, texelSize);
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float col = 0.0;
    col += 4*dot(texture(colorMap, co).rgb, luma);
    col -= dot(texture(colorMap, co + vec2(1.0, 0.0)*texelSize).rgb, luma);
    col -= dot(texture(colorMap, co + vec2(-1.0, 0.0)*texelSize).rgb, luma);
    col -= dot(texture(colorMap, co + vec2(0.0, 1.0)*texelSize).rgb, luma);
    col -= dot(texture(colorMap, co + vec2(0.0, -1.0)*texelSize).rgb, luma);
    return col > 0.15;
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
    UV[index(uv)] = currCoord;

    vec3 random = vec3(rand(uv*time));
    vec3 col = mix(texture(colorMap, uv).rgb, random, 0.7);

    int i = index(currCoord);
    atomicAdd(color[i].r, int(col.r*255));
    atomicAdd(color[i].g, int(col.g*255));
    atomicAdd(color[i].b, int(col.b*255));
    atomicAdd(color[i].a, checkEdge(uv) ? 255 : 1);

    return vec4(0.0);
} 

subroutine (stereoPass) vec4 secondPass() {
    int i = index(UV[index(uv)]);
    return vec4(color[i]/(color[i].w*255.0));
}

void main() {
    colorFrag = computePass();
}