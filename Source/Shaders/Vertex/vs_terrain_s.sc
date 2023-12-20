$input a_position, a_normal
#include <bgfx_shader.sh>

#define SHARED_SHADER
#include <Shared_TerrainFuncs.h>

void main() {
    a_position += u_terrainMeshOffset;
    vec2 position2d = a_position.xz;
    vec2 terrainDistance = getTerrainDistance(position2d);

    vec3 position = vec3(a_position.x, terrainDistance.y, a_position.z);
    gl_Position = mul(u_viewProj, vec4(position, 1.0f));
}
