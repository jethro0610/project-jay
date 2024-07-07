$input a_position, a_normal, a_tangent, a_bitangent, a_texcoord0
$output v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>

#define SHARED_SHADER
#include <Shared_TerrainFuncs.h>

uniform vec4 u_normalMult;

void main() {
    v_normal = mul(u_model[0], vec4(a_normal, 0.0f)).xyz; 
    v_normal = normalize(v_normal) * u_normalMult.x;

    v_bitangent = mul(u_model[0], vec4(a_bitangent, 0.0f)).xyz; 
    v_bitangent = normalize(v_bitangent) * u_normalMult.x;

    v_tangent = mul(u_model[0], vec4(a_tangent, 0.0f)).xyz; 
    v_tangent = normalize(v_tangent) * u_normalMult.x;

    v_tbn = mat3(v_tangent, v_bitangent, v_normal);

    v_texcoord0 = a_texcoord0;
    v_wposition = mul(u_model[0], vec4(a_position, 1.0f)).xyz;

    vec2 terrainDistance = getTerrainDistance(v_wposition.xz);
    v_wposition.y = terrainDistance.y + 0.1f;

    gl_Position = mul(u_viewProj, vec4(v_wposition, 1.0f));
}
