$input a_position, a_normal
$output v_wposition, v_edgeDistance, v_normal, v_tangent, v_bitangent, v_tbn
#include <bgfx_shader.sh>

#define SHARED_SHADER
#include <Shared_WorldFuncs.h>

void main() {
    WorldProperties props = {
        u_minHeight,
        u_minRadius,
        u_maxRadius,
        u_edgeJaggedness,
        u_edgeFalloff,
        u_edgePower,
        0.0f
    };

    a_position += u_worldMeshOffset;
    vec2 position2d = a_position.xz;

    vec2 worldDistance = getWorldDistance(position2d, props);
    v_wposition = vec3(a_position.x, worldDistance.y, a_position.z);
    v_edgeDistance = worldDistance.x;
    
    v_normal = getWorldNormal(position2d, props);
    v_tangent = -normalize(cross(vec3(1, 0, 0), v_normal));
    v_bitangent = normalize(cross(v_tangent, v_normal));
    v_tbn = mat3(v_tangent, v_bitangent, v_normal);

    gl_Position = mul(u_viewProj, vec4(v_wposition, 1.0f));
}
