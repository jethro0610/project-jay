$input v_wposition, v_normal, v_tangent, v_bitangent, v_tbn, v_texcoord0
#include <bgfx_shader.sh>

uniform vec4 u_cameraPosition;

SAMPLER2D(s_sampler0, 0);
SAMPLER2D(s_sampler1, 1);


float getSpecular(vec3 cameraPosition, vec3 wposition, vec3 lightDir, vec3 normal, float power) {
    vec3 viewDir = normalize(cameraPosition - wposition);
    vec3 reflectDir = reflect(-lightDir, normal);
    return pow(max(dot(-viewDir, reflectDir), 0.0f), power);
}

float getFresnel(vec3 cameraPosition, vec3 wposition, vec3 normal, float scale, float power) {
    vec3 viewDir = normalize(cameraPosition - wposition);
    return saturate(scale * pow(1.0f + dot(-viewDir, normal), power));
}

void main() {
    vec3 color = texture2D(s_sampler0, v_texcoord0).rgb;
    vec3 lightDir = normalize(vec3(1.0f, -1.0f, -1.0f));

    vec3 normal = texture2D(s_sampler1, v_texcoord0).rgb;
    normal = normal * 2.0f - 1.0f;
    normal = normalize(mul(normal, v_tbn));

    float ambient = 0.2f;
    float diffuse = max(-dot(normal, lightDir), 0.0f);
    float specular = getSpecular(u_cameraPosition.xyz, v_wposition, lightDir, normal, 32.0f);
    float fresnel = getFresnel(u_cameraPosition.xyz, v_wposition, normal, 1.0, 4.0f);
    float brightness = diffuse + ambient + specular;

    if (brightness <= 0.25f)
        brightness = 0.25f;
    else if (brightness <= 0.75f)
        brightness = 0.75f;
    else
        brightness = 1.00f;

    if (specular >= 0.3f)
      brightness = 1.5f;

    gl_FragColor = vec4(color * (brightness + fresnel), 1.0f);
}
