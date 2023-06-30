#version 430 core

layout (location = 0) in vec3 position;
out vec3 uvw;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
	uvw = position;
    vec4 pos = projectionMatrix * viewMatrix * vec4(position, 1.0);
	gl_Position = pos.xyww;
}
