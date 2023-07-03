#version 430 core

out vec4 colorFrag[2];
in vec3 uvw;

uniform samplerCube cubemap;
uniform float colorResolution;

void main() {
    colorFrag[0] = round(texture(cubemap, uvw)*255.0*pow(colorResolution, 3.0))/(255.0*pow(colorResolution, 3.0));
    colorFrag[1] = vec4(0.0);
}
