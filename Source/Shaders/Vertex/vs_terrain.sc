$input a_position, a_normal
$output v_wposition, v_sposition, v_edgeDistance, v_normal, v_tangent, v_bitangent, v_tbn
#include <bgfx_shader.sh>

#define SHARED_SHADER
#include <Shared_TerrainFuncs.h>

uniform vec4 u_normalMult;
uniform mat4 u_shadowMatrix;

void main() {
    a_position += u_terrainMeshOffset;
    vec2 position2d = a_position.xz;

    vec2 terrainDistance = getTerrainDistance(position2d);
    v_wposition = vec3(a_position.x, terrainDistance.y, a_position.z);
    v_edgeDistance = terrainDistance.x;
    
    v_normal = getTerrainNormal(position2d) * u_normalMult.x;
    v_tangent = -normalize(cross(vec3(1, 0, 0), v_normal));
    v_bitangent = normalize(cross(v_tangent, v_normal));
    v_tbn = mat3(v_tangent, v_bitangent, v_normal);

    v_sposition = mul(u_shadowMatrix, vec4(v_wposition, 1.0f));
    gl_Position = mul(u_viewProj, vec4(v_wposition, 1.0f));
}
