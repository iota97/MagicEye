#version 430 core

out vec4 colorFrag;

in vec3 lightDir;
in vec3 vNormal;
in vec3 vViewPosition;
in vec2 uv;

uniform sampler2D tex;
uniform float repeat;

void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(lightDir.xyz);
    vec3 V = normalize(vViewPosition);
    vec3 H = normalize(L + V);

    float NdotL = max(dot(N, L), 0.1);
    float specAngle = max(dot(H, N), 0.0);
    float specular = pow(specAngle, 300.0);

    colorFrag = texture(tex, uv*repeat)*(NdotL + specular);
}
