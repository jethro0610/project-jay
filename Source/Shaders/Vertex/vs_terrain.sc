$input a_position, a_normal
$output v_wposition, v_sposition, v_edgeDistance, v_normal, v_tangent, v_bitangent, v_tbn
#include <bgfx_shader.sh>
#include <noise.sh>

#define SHARED_SHADER
#include <Shared_TerrainFuncs.h>

uniform vec4 u_normalMult;
uniform mat4 u_shadowMatrix;
uniform vec4 u_time;

void main() {
    a_position += u_terrainMeshOffset;
    vec2 position2d = a_position.xz;
    vec2 terrainDistance = getTerrainDistance(position2d);

    vec3 normal = getTerrainNormal(position2d) * u_normalMult.x;
    v_normal = normal;

    vec3 worldPos = vec3(a_position.x, terrainDistance.y, a_position.z);
    fnl_state noise = fnlCreateState(1337);
    noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
    float baseVal = fnlGetNoise3D(
        noise, 
        worldPos.x, 
        worldPos.y, 
        worldPos.z
    );
    float noiseVal = fnlGetNoise3D(
        noise, 
        worldPos.x + sin(u_time.x * 0.5f + 10.0f) * 20.0f, 
        worldPos.y + sin(u_time.x * 0.2f + 20.0f) * 20.0f, 
        worldPos.z + sin(u_time.x * 0.4f + 30.0f) * 20.0f
    );
    noiseVal = (noiseVal + 1.0f) * 0.5f;
    baseVal = (baseVal + 1.0f) * 0.5f;
    noiseVal *= max(terrainDistance.x, 0.0f) * 0.1f;
    baseVal *= max(terrainDistance.x, 0.0f) * 0.1f;
    v_wposition = worldPos + normal * (baseVal * 10.0f);

    v_edgeDistance = terrainDistance.x;
    
    v_tangent = -normalize(cross(vec3(1, 0, 0), v_normal));
    v_bitangent = normalize(cross(v_tangent, v_normal));
    v_tbn = mat3(v_tangent, v_bitangent, v_normal);

    v_sposition = mul(u_shadowMatrix, vec4(v_wposition, 1.0f));
    gl_Position = mul(u_viewProj, vec4(v_wposition, 1.0f));
}
