#version 430 core

out vec4 colorFrag;

in vec3 lightDir;
in vec3 vNormal;
in vec2 uv;

uniform sampler2D tex;
uniform float repeat;

void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(lightDir.xyz);
    colorFrag = texture(tex, uv*repeat)*max(dot(N, L), 0.1);
}
