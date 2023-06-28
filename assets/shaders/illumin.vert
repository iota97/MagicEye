#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 UV;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightVector;
out vec3 lightDir;
out vec3 vNormal;
out vec2 uv;

void main() {
  vec4 mvPosition = viewMatrix * modelMatrix * vec4(position, 1.0);
  vNormal = normalize(normalMatrix * normal);
  lightDir = vec3(viewMatrix * vec4(lightVector, 0.0));
  gl_Position = projectionMatrix * mvPosition;
  uv = UV;
}