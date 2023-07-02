$input a_position, a_normal
$output v_wposition, v_normal, v_tangent, v_bitangent, v_tbn
#include <bgfx_shader.sh>
#include <NoiseSample.sh>
#include <WorldUniform.sh>


float getHeight(vec3 position) {
    vec2 position2d = vec2(position.x, position.z);

    float blobVal = noiseSampleBlob(position2d, u_edgeJaggedness);
    blobVal = (blobVal + 1.0f) * 0.5f;
    float blobRadius = u_minRadius + blobVal * (u_maxRadius - u_minRadius);
    float curRadius = length(position2d);
    float edgeCloseness = max(1.0f - (blobRadius - curRadius) * u_edgeFalloff, 0.0f);
    float edgeHeight = -pow(edgeCloseness, u_edgePower);

    float terrainVal = noiseSample(position2d, 0.75f);
    terrainVal = (terrainVal + 1.0f) * 0.5f;
    float terrainHeight = terrainVal * 12.0f;

    float d = 0;
    for (int i = 0; i < 128; i++) {
        float modifier = noiseSample(position2d, 1.0f);
        d += distance(vec2(0.0f, 0.0f), vec2(modifier, modifier));
        d *= 0.25f;
        d = pow(d, 6.0f);
    }

    return u_minHeight + terrainHeight + edgeHeight + d - d;
}

void main() {
    a_position += u_worldMeshOffset;
    float height = getHeight(a_position);
    // float height = 0.0f;
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
