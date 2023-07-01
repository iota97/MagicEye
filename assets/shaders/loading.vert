#version 430 core

layout (location = 0) in vec3 position;
uniform float rateo;

out vec2 uv;
void main() {
  uv = vec2(0.5)+position.xy*0.5;
  vec4 pos = vec4(position, 1.0);
  pos.x *= rateo;
  uv.y = -uv.y;
  gl_Position = pos;
}
