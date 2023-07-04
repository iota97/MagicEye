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
uniform mat4 finalBonesMatrices[MAX_BONES];
	
out vec3 lightDir;
out vec3 vNormal;
out vec2 uv;
out vec3 vViewPosition;
	
void main() {
    mat4 totalTrasform = mat4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++) {
        if(boneIds[i] == -1) 
            continue;

        if(boneIds[i] >= MAX_BONES) {
            totalTrasform = mat4(1.0f);
            break;
        }

        totalTrasform += finalBonesMatrices[boneIds[i]] * weights[i];
    }
		
    vec4 mvPosition = viewMatrix * modelMatrix * totalTrasform * vec4(pos, 1.0);
    vViewPosition = -mvPosition.xyz;
    vNormal = normalize(normalMatrix * inverse(transpose(mat3(totalTrasform))) * norm);
    lightDir = vec3(viewMatrix * vec4(lightVector, 0.0));
    gl_Position =  projectionMatrix * mvPosition;
    uv = tex;
}