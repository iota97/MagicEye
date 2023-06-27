#version 430 core

out vec4 colorFrag;
in vec2 uv;
uniform sampler2D depthMap;
uniform sampler2D colorMap;
uniform sampler2D random;
uniform float final;

layout (std430, binding = 0) coherent buffer Color
{
  ivec4 color[];
};

float sampleDepth(vec2 uv) {
    return pow(texture(depthMap, uv).r, 5.0);
}

int index(vec2 co) {
    int width = 1280-1;
    int height = 720-1;
    return int((co.y*height+co.x)*width);
}

void main() {
        vec2 currCoord = uv;
        for(int i = 0; i < 32; i++) {
            float depth = sampleDepth(currCoord);
            float sep = 0.5*depth/(depth + 1.8);
            if(currCoord.x < 0.0)
                break;
            currCoord.x -= sep;       
        }
        currCoord.x = mod(currCoord.x, 1.0);

        vec4 col = mix(texture(random, currCoord*8.0), texture(colorMap, uv), 0.4);

        if (final > 0.5) {
            colorFrag = color[index(currCoord)]/(color[index(currCoord)].w*255.0);
        } else {
            atomicAdd(color[index(currCoord)].r, int(col.r*255));
            atomicAdd(color[index(currCoord)].g, int(col.g*255));
            atomicAdd(color[index(currCoord)].b, int(col.b*255));
            atomicAdd(color[index(currCoord)].a, 1);
            colorFrag = vec4(0.5);
        }
}