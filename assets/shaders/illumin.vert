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
out vec3 vViewPosition;
out vec2 uv;

void main() {
  // vertex position in world coordinates
  vec4 mPosition = modelMatrix * vec4( position, 1.0 );
  // vertex position in camera coordinates
  vec4 mvPosition = viewMatrix * mPosition;

  // view direction, negated to have vector from the vertex to the camera
  vViewPosition = -mvPosition.xyz;

  // transformations are applied to the normal
  vNormal = normalize( normalMatrix * normal );

  // light incidence directions in view coordinate
  lightDir = vec3(viewMatrix  * vec4(lightVector, 0.0));

  // we apply the projection transformation
  gl_Position = projectionMatrix * mvPosition;

  // I assign the values to a variable with "out" qualifier so to use the per-fragment interpolated values in the Fragment shader
  uv = UV;
}