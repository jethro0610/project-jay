$input a_position, a_normal
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

    vec3 position = vec3(a_position.x, worldDistance.y, a_position.z);
    gl_Position = mul(u_viewProj, vec4(position, 1.0f));
}
