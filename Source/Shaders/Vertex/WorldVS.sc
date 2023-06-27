$input a_position, a_normal
$output v_wposition, v_normal, v_tangent, v_bitangent, v_tbn
#include <bgfx_shader.sh>

SAMPLER2D(s_sampler0, 0);

static float MAX_NOISE_POS = 256.0f;
static float SAMPLE_SCALE = 1.0f / (MAX_NOISE_POS * 2.0f);

float getHeight(vec3 position) {
    vec2 samplePos = vec2(position.x * SAMPLE_SCALE * 0.75f, position.z * SAMPLE_SCALE * 0.75f);
    samplePos += vec2(0.5f, 0.5f);
    return texture2DLod(s_sampler0, samplePos, 0) * 8.0f;
}

void main() {
    float height = getHeight(a_position);
    v_wposition = vec3(a_position.x, height, a_position.z);

    vec3 dx = a_position - vec3(1.0f, 0.0f, 0.0f);
    vec3 dz = a_position - vec3(0.0f, 0.0f, 1.0f);
    float gradX = getHeight(dx) - height;
    float gradZ = getHeight(dz) - height;

    v_normal = normalize(vec3(gradX, 1.0f, gradZ));
    v_tangent = -normalize(cross(vec3(1, 0, 0), v_normal));
    v_bitangent = normalize(cross(v_tangent, v_normal));
    v_tbn = mat3(v_tangent, v_bitangent, v_normal);

    gl_Position = mul(u_viewProj, vec4(v_wposition, 1.0f));
}
