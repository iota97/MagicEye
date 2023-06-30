#version 430 core

out vec4 colorFrag;
in vec3 uvw;
uniform samplerCube cubemap;

void main() {
    colorFrag = texture(cubemap, uvw);
}
