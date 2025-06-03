$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <lighting.sh>
#include <properties.sh>
#define SHARED_SHADER
#include <Shared_TerrainFuncs.h>

uniform vec4 u_cameraForward;
uniform vec4 u_cameraUp;
uniform vec4 u_cameraRight;

void main() {
    gl_FragDepth = 0.0f;

    float aspect = 16.0f / 9.0f;
    float fov = 70.0f;
    float fov2 = radians(fov) / 2.0f;

    vec2 rayUv = vec2(
        gl_FragCoord.x / 1920,
        gl_FragCoord.y / 1080
    );
    rayUv -= 0.5f;
    rayUv *= 2.0f;
    rayUv.x *= 16.0f / 9.0f;
    rayUv *= tan(fov2);

    vec3 rayDirection = 
        rayUv.x * u_cameraRight.xyz +
        rayUv.y * -u_cameraUp.xyz +
        u_cameraForward.xyz;
    rayDirection = normalize(rayDirection);

    vec3 circleOrigin = PROP_RAYMARCH_ORIGIN;
    float circleRadius = PROP_RAYMARCH_RADIUS * 0.5f;

    vec3 pos = u_cameraPosition.xyz;
    bool hit = false;
    for (int i = 0; i < 32; i++) {
        // float dist = distance(pos, circleOrigin) - circleRadius;
        float dist = getTerrainDistance(pos.xz).x;
        float absDist = abs(dist);
        if (absDist < 0.1f) {
            hit = true;
            break;
        }
        pos += rayDirection * absDist;
    }

    if (hit) {
        vec4 clipPos = mul(u_viewProj, vec4(pos, 1.0f));
        float d = clipPos.z / clipPos.w;

        gl_FragDepth = d;
        gl_FragColor = vec4(
            d,
            d,
            d,
            1.0f
        );
    }
    else
        discard;
}
