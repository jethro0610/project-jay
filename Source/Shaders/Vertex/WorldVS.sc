$input a_position, a_normal
$output v_wposition, v_normal, v_tangent, v_bitangent, v_tbn
#include <bgfx_shader.sh>

uniform vec4 u_worldProps[2];
#define u_maxNoiseDistance u_worldProps[0].x
#define u_minHeight u_worldProps[0].y
#define u_minRadius u_worldProps[0].z
#define u_maxRadius u_worldProps[0].w
#define u_edgeJaggedness u_worldProps[1].x
#define u_edgeFalloff u_worldProps[1].y
#define u_edgePower u_worldProps[1].z
#define sampleScale (1.0f / (u_maxNoiseDistance * 2.0f))

SAMPLER2D(s_sampler0, 0);

float getHeight(vec3 position) {
    vec2 position2d = vec2(position.x, position.z);

    vec2 blobSamplePos = normalize(position2d) * sampleScale * u_edgeJaggedness;
    blobSamplePos += vec2(0.5f, 0.5f);
    float blobVal = texture2DLod(s_sampler0, blobSamplePos, 0);
    blobVal = (blobVal + 1.0f) * 0.5f;

    float blobRadius = u_minRadius + blobVal * (u_maxRadius - u_minRadius);
    float curRadius = length(position2d);
    float edgeCloseness = max(1.0f - (blobRadius - curRadius) * u_edgeFalloff, 0.0f);
    float edgeHeight = -pow(edgeCloseness, u_edgePower);

    vec2 terrainSamplePos = position2d * sampleScale * 0.75f;
    terrainSamplePos += vec2(0.5f, 0.5f); 
    float terrainVal = texture2DLod(s_sampler0, terrainSamplePos, 0);
    terrainVal = (terrainVal + 1.0f) * 0.5f;
    float terrainHeight = terrainVal * 16.0f;

    return u_minHeight + terrainHeight + edgeHeight;
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
