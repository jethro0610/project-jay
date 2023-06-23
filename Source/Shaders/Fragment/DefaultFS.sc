$input v_worldPosition, v_normal, v_tangent, v_bitangent, v_texcoord
#include <bgfx_shader.sh>

uniform mat4 u_cameraPosition;

SAMPLER2D(s_sampler0, 0);
SAMPLER2D(s_sampler1, 1);

float getSpecular(vec3 lightDir, vec3 normal, float power) {
    vec3 viewDir = normalize(u_cameraPosition - v_worldPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    return pow(max(dot(-viewDir, reflectDir), 0.0f), power);
}

float getFresnel(vec3 lightDir, vec3 normal, float scale, float power) {
    vec3 viewDir = normalize(u_cameraPosition - v_worldPosition);
    return saturate(scale * pow(1.0f + dot(-viewDir, normal), power));
}

void main() {
    mat3 tbn = mat3(v_tangent, v_bitangent, v_normal);
    vec4 color = texture2D(s_sampler0, v_texcoord)

    float3 normal = texture2D(s_sampler1, v_texcoord)
    normal = normalize(normal * 2.0f - 1.0f);
    normal = normalize(mul(normal, tbn));

    float3 lightDir = float3(1.0, -1.0f, -1.0f);
    lightDir = normalize(lightDir);

    float ambient = 0.2f;
    float diffuse = max(-dot(normal, lightDir), 0.0f);
    float specular = getSpecular(lightDir, normal, 32.0f);
    float fresnel = getFresnel(lightDir, normal, 1.0f, 4.0f);
    float brightness = diffuse + ambient + specular;
    
    if (brightness <= 0.25f)
        brightness = 0.25f;
    else if (brightness <= 0.75f)
        brightness = 0.75f;
    else
        brightness = 0.85f;
    if (specular >= 0.25f)
        brightness = 1.25f;

    gl_FragColor = color * (brightness + fresnel);
}
