$input a_position, i_data0
#include <bgfx_shader.sh>

#define SHARED_SHADER
#include <Shared_TerrainFuncs.h>

#define i_offset i_data0.xyz

void main() {
    a_position += i_offset;
    vec2 position2d = a_position.xz;
    vec2 terrainDistance = getTerrainDistance(position2d);

    vec3 position = vec3(a_position.x, terrainDistance.y, a_position.z);
    gl_Position = mul(u_viewProj, vec4(position, 1.0f));
}
