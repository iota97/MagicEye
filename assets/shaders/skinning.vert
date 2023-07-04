#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;
layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;
	
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightVector;
	
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform vec4 finalPrimal[MAX_BONES];
uniform vec4 finalDual[MAX_BONES];
uniform vec3 finalScale[MAX_BONES];
uniform mat4 finalBonesMatrices[MAX_BONES];

out vec3 lightDir;
out vec3 vNormal;
out vec2 uv;
out vec3 vViewPosition;

vec4 q_mul(vec4 a, vec4 b) {
    return vec4(a.xyz*b.w+b.xyz*a.w+cross(a.xyz, b.xyz), a.w*b.w-dot(a.xyz, b.xyz));
}

vec4 p_mul(vec4 ap, vec4 bp) {
    return q_mul(ap, bp);
}

vec4 d_mul(vec4 ap, vec4 ad, vec4 bp, vec4 bd) {
    return q_mul(ap, bd) + q_mul(ad, bp);
}

subroutine vec4 skin();
subroutine uniform skin skinning;

subroutine (skin) vec4 dualQuat() {
    vec4 totalP = vec4(0.0f);
    vec4 totalD = vec4(0.0f);
    vec3 scale = vec3(0.0f);
    
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++) {
        if (boneIds[i] == -1) 
            continue;

        if (boneIds[i] >= MAX_BONES) {
            totalP = vec4(0.0f, 0.0f, 0.0f, 1.0f);
            totalD = vec4(0.0f);
            scale = vec3(1.0f);
            break;
        }
        scale += finalScale[boneIds[i]] * weights[i];
        
        float si = 1.0;
        if (dot(finalPrimal[boneIds[i]], totalP) < 0) {
            si = -1.0;
        }
        totalP += si * finalPrimal[boneIds[i]] * weights[i];
        totalD += si * finalDual[boneIds[i]] * weights[i];
    }    

    // Normalize
    float len = length(totalP);
    totalP /= len;
    totalD /= len;
    totalD -= dot(totalP, totalD)*totalP;

    // Apply the transform to the vertex
    vec4 totalPcon = vec4(-totalP.xyz, totalP.w);
    vec4 totalDcon = vec4(totalD.xyz, -totalD.w);

    vec4 posP = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 posD = vec4(scale*pos, 0.0);

    posD = d_mul(totalP, totalD, posP, posD);
    posP = p_mul(totalP, posP);
    posD = d_mul(posP, posD, totalPcon, totalDcon);

    vNormal = normalize(normalMatrix * q_mul(q_mul(totalP, vec4(norm, 0.0)), totalPcon).xyz);
    return vec4(posD.xyz, 1.0);
}

subroutine (skin) vec4 matrix() {
    mat4 totalTrasform = mat4(0.0f);
    
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++) {
        if (boneIds[i] == -1) 
            continue;

        if (boneIds[i] >= MAX_BONES) {
            totalTrasform = mat4(1.0f);
            break;
        }

        totalTrasform += finalBonesMatrices[boneIds[i]] * weights[i];
    }    

    vNormal = normalize(normalMatrix * inverse(transpose(mat3(totalTrasform))) * norm);
    return totalTrasform * vec4(pos, 1.0);
}
	
void main() {
    vec4 mvPosition = viewMatrix * modelMatrix * skinning();
    vViewPosition = -mvPosition.xyz;
    lightDir = vec3(viewMatrix * vec4(lightVector, 0.0));
    gl_Position =  projectionMatrix * mvPosition;
    uv = tex;
}