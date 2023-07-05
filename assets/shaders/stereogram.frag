#version 430 core

in vec2 uv;
out vec4 colorFrag;

uniform sampler2D depthTex;
uniform sampler2D colorTex;
uniform sampler2D normalTex;
uniform sampler2D randomTexture;

uniform int bufferWidth;
uniform int bufferHeight;
uniform float time;
uniform float eyeSep;
uniform float obsDistance;
uniform float edgeStr;
uniform float sceneColorStr;
uniform float depthStrength;
uniform float edgeThreshold;
uniform bool edgeNormal;

layout (std430, binding = 0) coherent buffer Color {
  ivec4 color[];
};

layout (std430, binding = 1) coherent buffer Uv {
  vec2 UV[];
};

layout (std430, binding = 2) coherent buffer Edge {
  int edge[];
};

subroutine vec3 randomPattern(vec2 co);
subroutine uniform randomPattern pattern;

subroutine vec4 stereoPass();
subroutine uniform stereoPass computePass;

vec3 mod289(vec3 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
      return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r) {
    return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v) {
    const vec2 C = vec2(1.0/6.0, 1.0/3.0) ;
    const vec4 D = vec4(0.0, 0.5, 1.0, 2.0);
    vec3 i = floor(v + dot(v, C.yyy) );
    vec3 x0 = v - i + dot(i, C.xxx) ;
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 i1 = min( g.xyz, l.zxy );
    vec3 i2 = max( g.xyz, l.zxy );
    vec3 x1 = x0 - i1 + C.xxx;
    vec3 x2 = x0 - i2 + C.yyy; 
    vec3 x3 = x0 - D.yyy;
    i = mod289(i);
    vec4 p = permute( permute( permute(
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));
    float n_ = 0.142857142857;
    vec3 ns = n_ * D.wyz - D.xzx;
    vec4 j = p - 49.0 * floor(p * ns.z * ns.z);
    vec4 x_ = floor(j * ns.z);
    vec4 y_ = floor(j - 7.0 * x_ );
    vec4 x = x_ *ns.x + ns.yyyy;
    vec4 y = y_ *ns.x + ns.yyyy;
    vec4 h = 1.0 - abs(x) - abs(y);
    vec4 b0 = vec4( x.xy, y.xy );
    vec4 b1 = vec4( x.zw, y.zw );
    vec4 s0 = floor(b0)*2.0 + 1.0;
    vec4 s1 = floor(b1)*2.0 + 1.0;
    vec4 sh = -step(h, vec4(0.0));
    vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
    vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;
    vec3 p0 = vec3(a0.xy,h.x);
    vec3 p1 = vec3(a0.zw,h.y);
    vec3 p2 = vec3(a1.xy,h.z);
    vec3 p3 = vec3(a1.zw,h.w);
    vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;
    vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
    m = m * m;
    return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),
                                dot(p2,x2), dot(p3,x3) ) );
}

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

int index(vec2 co) {
    int width = bufferWidth-1;
    int height = bufferHeight-1;
    int res = int((co.y*height+co.x)*width);
    return (res >= 0 && res < bufferWidth*bufferHeight) ? res : 0;
}

float sampleDepth(vec2 uv) {
    return pow(texture(depthTex, uv).r, depthStrength);
}


void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord) {
	float w = 1.0 / textureSize(tex, 0).x;
	float h = 1.0 / textureSize(tex, 0).y;

	n[0] = texture(tex, coord + vec2( -w, -h));
	n[1] = texture(tex, coord + vec2(0.0, -h));
	n[2] = texture(tex, coord + vec2(  w, -h));
	n[3] = texture(tex, coord + vec2( -w, 0.0));
	n[4] = texture(tex, coord);
	n[5] = texture(tex, coord + vec2(  w, 0.0));
	n[6] = texture(tex, coord + vec2( -w, h));
	n[7] = texture(tex, coord + vec2(0.0, h));
	n[8] = texture(tex, coord + vec2(  w, h));
}

bool checkEdge(vec2 co) {
    vec4 n[9];

    // no ternary on sample on AMD...
    if (edgeNormal) {
	    make_kernel(n, normalTex , uv);
    } else {
        make_kernel(n, colorTex, uv);
    }

	vec4 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
  	vec4 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
	vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

	return length(sobel.rgb) > edgeThreshold;
}

subroutine (randomPattern) vec3 perlinNoise(vec2 co) {
    vec3 random = vec3(0.3);
    random += vec3(snoise(vec3(co*32.0, time)));
    random += 0.5*vec3(snoise(vec3(co*64.0, time*2.0)));
    random += 0.25*vec3(snoise(vec3(co*128.0, time*4.0)));
    random += 0.125*vec3(snoise(vec3(co*256.0, time*8.0)));
    return random;
} 

subroutine (randomPattern) vec3 perlinNoiseRGB(vec2 co) {
    vec3 random = vec3(0.3);
    random += vec3(snoise(vec3(co*32.0, time)), 
            snoise(vec3(co*32.0, time+16.0)), 
            snoise(vec3(co*32.0, time+32.0)));
    random += 0.5*vec3(snoise(vec3(co*64.0, time*2.0)),
            snoise(vec3(co*64.0, time*2.0-16.0)),
            snoise(vec3(co*64.0, time*2.0-32.0)));
    random += 0.25*vec3(snoise(vec3(co*128.0, time*4.0)),
            snoise(vec3(co*128.0, time*4.0+2.0)),
            snoise(vec3(co*128.0, time*4.0-2.0)));
    random += 0.125*vec3(snoise(vec3(co*256.0, time*8.0)),
            snoise(vec3(co*256.0, time*8.0+7.0)),
            snoise(vec3(co*256.0, time*8.0-7.0)));
    return random;
} 

subroutine (randomPattern) vec3 texturePattern(vec2 co) {
    return texture(randomTexture, co*6.0+vec2(cos(time*0.2), sin(time*0.2))).rgb;
} 

subroutine (stereoPass) vec4 firstPass() {
    vec2 currCoord = uv;
    for(int i = 0; i < 32; i++) {
        float depth = sampleDepth(currCoord);
        float sep = eyeSep*depth/(depth + obsDistance);
        if(currCoord.x < 0.0)
           break;
        currCoord.x -= sep;
    }
    currCoord.x = mod(currCoord.x, 1.0);
    UV[index(uv)] = currCoord;

    vec4 col = texture(colorTex, uv);
    col.rgb *= col.a;
    int i = index(currCoord);
    atomicAdd(color[i].r, int(col.r*4096));
    atomicAdd(color[i].g, int(col.g*4096));
    atomicAdd(color[i].b, int(col.b*4096));
    atomicAdd(color[i].a, int(col.a*4096));

    if (checkEdge(uv)) {
        atomicExchange(edge[i], 1);
    }
    return vec4(0.0);
} 

subroutine (stereoPass) vec4 secondPass() {
    vec2 co = UV[index(uv)];
    int i = index(co);
    vec3 random = pattern(co);
    vec3 col = vec3(0.0);
    if (color[i].w != 0) {
        col = color[i].rgb/float(color[i].w);
    }
    col = mix(col, random, 1.0-sceneColorStr);
    float edgeFactor = bool(edge[i]) ? 1.0-edgeStr : 1.0;
    return vec4(col*edgeFactor, 1.0);
}

void main() {
    colorFrag = computePass();
}