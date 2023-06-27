$input v_wposition, v_normal
#include <bgfx_shader.sh>

SAMPLER2D(s_sampler0, 0);
SAMPLER2D(s_sampler1, 1);

static float MAX_NOISE_POS = 256.0f;
static float SAMPLE_SCALE = 1.0f / (MAX_NOISE_POS * 2.0f);

void main() {
    vec3 lightDir = normalize(vec3(1.0f, -1.0f, -1.0f));
    float diffuse = max(-dot(v_normal, lightDir), 0.0f);

    vec3 color = texture2D(s_sampler1, vec2(v_wposition.x, v_wposition.z) / 128.0f).rgb;
    gl_FragColor = vec4(color * diffuse, 1.0f);
}
