$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <lighting.sh>
#include <properties.sh>

#define SHARED_SHADER
#include <Shared_TerrainFuncs.h>

void main() {
    vec3 normal = v_normal;
    float brightness = getBrightness(normal, v_wposition, v_sposition);

    vec2 position2d = v_wposition.xz;
    vec2 terrainDistance = getTerrainDistance(position2d);

    float alpha = step(terrainDistance.y, v_wposition.y);
    gl_FragColor = vec4(PROP_COLOR.xyz * brightness, alpha);
}
