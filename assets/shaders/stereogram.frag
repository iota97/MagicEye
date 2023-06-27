#version 430 core

out vec4 colorFrag;
in vec2 uv;
uniform sampler2D depthMap;
uniform sampler2D colorMap;
uniform sampler2D random;

float sampleDepth(vec2 uv) {
    return pow(texture(depthMap, uv).r, 4.0);
}

vec3 genColor(vec2 coord) {
    vec2 currCoord = coord;
    for(int i = 0; i < 32; i++) {
     	float depth = sampleDepth(currCoord);
        float sep = 0.5*depth/(depth + 1.8);
        if(currCoord.x < 0.0)
            break;
        currCoord.x -= sep;       
    }
    currCoord.x = mod(currCoord.x, 1.0);
    return texture(random, currCoord*8.0).rgb;
}

void main() {
    colorFrag = vec4(genColor(uv),1.0);
    //colorFrag = texture(colorMap, uv);
}
