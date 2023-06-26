$input v_wposition, v_normal, v_tangent, v_bitangent, v_tbn, v_texcoord0
#include <bgfx_shader.sh>

SAMPLER2D(s_sampler0, 0);
SAMPLER2D(s_sampler1, 1);

void main() {
    vec3 color = texture2D(s_sampler0, v_texcoord0).rgb;
    vec3 lightDir = normalize(vec3(1.0f, -1.0f, -1.0f));

    vec3 normal = texture2D(s_sampler1, v_texcoord0).rgb;
    normal = normal * 2.0f - 1.0f;
    normal = normalize(mul(normal, v_tbn));

    float diffuse = max(-dot(normal, lightDir), 0.0f);

    gl_FragColor = vec4(color * diffuse, 1.0f);
}
