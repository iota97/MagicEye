#version 430 core

out vec4 colorFrag[2];

in vec3 lightDir;
in vec3 vNormal;
in vec2 uv;
in vec3 vViewPosition;

uniform sampler2D tex;
uniform float alphaMultiplier;

uniform float Ka;
uniform float Kd;
uniform float Ks;

uniform vec3 specularColor;
uniform float shininess;
uniform float colorResolution;

subroutine vec3 illum();
subroutine uniform illum illumModel;

subroutine (illum) vec3 phong() {
    vec3 albedo = texture(tex, uv).rgb;
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
  
    return color;
}

subroutine (illum) vec3 toon() {
    vec3 albedo = texture(tex, uv).rgb;
    vec3 color = Ka*albedo;
    vec3 N = normalize(vNormal);
    vec3 L = normalize(lightDir);
    float lambertian = step(0.5, max(dot(L,N), 0.0));

    if (lambertian > 0.0) {
      vec3 V = normalize(vViewPosition);
      vec3 H = normalize(L + V);
      float specAngle = max(dot(H, N), 0.0);
      float specular = step(0.2, pow(specAngle, shininess));

      color += Kd*lambertian*albedo;
      color += Ks*specular*specularColor;
    }
  
    return color;
}

void main() {
  colorFrag[0] = vec4(round(illumModel()*255.0*pow(colorResolution, 3.0))/(255.0*pow(colorResolution, 3.0)), alphaMultiplier);
  colorFrag[1] = vec4(vNormal, 1.0);
}
