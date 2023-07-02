#version 430 core

out vec4 colorFrag;
in vec3 uvw;

uniform samplerCube cubemap;
uniform float colorResolution;

void main() {
    colorFrag = round(texture(cubemap, uvw)*255.0*pow(colorResolution, 3.0))/(255.0*pow(colorResolution, 3.0));
}
