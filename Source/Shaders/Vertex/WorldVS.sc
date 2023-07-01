$input a_position, a_normal
$output v_wposition, v_normal, v_tangent, v_bitangent, v_tbn
#include <bgfx_shader.sh>

SAMPLER2D(s_sampler0, 0);

static float MAX_NOISE_POS = 256.0f;
static float SAMPLE_SCALE = 1.0f / (MAX_NOISE_POS * 2.0f);

float getHeight(vec3 position) {
    vec2 position2d = vec2(position.x, position.z);
    float BLOB_SAMPLE_RADIUS = 64.0f;

    vec2 blobSamplePos = normalize(position2d) * SAMPLE_SCALE * BLOB_SAMPLE_RADIUS;
    blobSamplePos += vec2(0.5f, 0.5f);
    float blobVal = texture2DLod(s_sampler0, blobSamplePos, 0);
    blobVal = (blobVal + 1.0f) * 0.5f;

    float blobRadius = 32.0f + blobVal * 64.0f;
    float curRadius = length(position2d);
    float edgeCloseness = max(1.0f - (blobRadius - curRadius) * 0.25f, 0.0f);
    edgeCloseness = pow(edgeCloseness, 1.5f);

    // return texture2DLod(s_sampler0, samplePos, 0) * 8.0f;
    return 0.0f - edgeCloseness;
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
