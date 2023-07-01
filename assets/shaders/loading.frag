#version 430 core

in vec2 uv;
out vec4 colorFrag;
uniform sampler2D tex;

void main() {
    colorFrag = texture(tex, uv);
}
