$input v_wposition, v_normal, v_tangent, v_bitangent, v_texcoord0
#include <bgfx_shader.sh>

SAMPLER2D(s_sampler0, 0);

void main() {
    vec3 color = texture2D(s_sampler0, v_texcoord0).rgb;
    vec3 lightDir = normalize(vec3(1.0f, -1.0f, -1.0f));

    float diffuse = max(-dot(v_normal, lightDir), 0.0f);

    gl_FragColor = vec4(color * diffuse, 1.0f);
}
