#version 430 core

out vec4 colorFrag;

in vec3 lightDir;
in vec3 vNormal;
in vec2 uv;
in vec3 vViewPosition;

uniform sampler2D tex;
uniform float repeat;

uniform float Ka;
uniform float Kd;
uniform float Ks;

uniform vec3 specularColor;
uniform float shininess;

void main() {
    vec3 albedo = texture(tex, uv*repeat).rgb;
    vec3 color = Ka*albedo;
    vec3 N = normalize(vNormal);
    vec3 L = normalize(lightDir);
    float lambertian = max(dot(L,N), 0.0);

    if (lambertian > 0.0) {
      vec3 V = normalize(vViewPosition);
      vec3 H = normalize(L + V);
      float specAngle = max(dot(H, N), 0.0);
      float specular = pow(specAngle, shininess);

      color += Kd*lambertian*albedo;
      color += Ks*specular*specularColor;
    }

    colorFrag = vec4(color, 1.0);
}
