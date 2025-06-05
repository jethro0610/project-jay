$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <lighting.sh>
#include <properties.sh>
#define SHARED_SHADER
#include <Shared_TerrainFuncs.h>

uniform vec4 u_cameraForward;
uniform vec4 u_cameraUp;
uniform vec4 u_cameraRight;
SAMPLER2D(s_sampler0, 0);

vec3 project2Dto3D(vec2 a, vec3 b) {
    vec3 a3 = vec3(a.x, 0.0f, a.y);
    return (dot(a3, b) / dot(b, b)) * b;
}

void main() {
    float aspect = 16.0f / 9.0f;
    float fov = 70.0f;
    float fov2 = radians(fov) / 2.0f;

    vec2 rayUv = vec2(
        gl_FragCoord.x / 1920,
        gl_FragCoord.y / 1080
    );
    float depth = texture2D(s_sampler0, rayUv).r;
    rayUv -= 0.5f;
    rayUv *= 2.0f;
    rayUv.x *= 16.0f / 9.0f;
    rayUv *= tan(fov2);

    vec3 rayDirection = 
        rayUv.x * u_cameraRight.xyz +
        rayUv.y * -u_cameraUp.xyz +
        u_cameraForward.xyz;
    rayDirection = normalize(rayDirection);
    vec2 rayDirection2D = normalize(rayDirection.xz);


    vec3 pos = u_cameraPosition.xyz;
    bool hit = false;
    bool inHit = false;
    if (-SAMPLETERRAINMAP(pos.xz).x < 0.0f && distance(PROP_RAYMARCH_ORIGIN.xz, pos.xz) < PROP_RAYMARCH_RADIUS) {
        hit = true;
        inHit = true;
    }

    for (int i = 0; i < 32; i++) {
        if (hit)
            break;
        float distFromVolume = distance(PROP_RAYMARCH_ORIGIN.xz, pos.xz) - PROP_RAYMARCH_RADIUS;
        if (distFromVolume < 0.1f)
            break;
        pos += project2Dto3D(rayDirection2D * distFromVolume, rayDirection);
    }

    for (int i = 0; i < 32; i++) {
        float dist = -SAMPLETERRAINMAP(pos.xz).x;
        if (dist < 0.25f) {
            hit = true;
            break;
        }
        pos += project2Dto3D(rayDirection2D * dist, rayDirection);
    }

    float stepsInside = 0;
    if (hit) {
        vec3 internalStepPos = pos + rayDirection * 0.25f;
        int seed = 1337;
        for (int i = 0; i < 1024; i++) {
            vec4 clipPos = mul(u_viewProj, vec4(internalStepPos, 1.0f));
            float d = clipPos.z / clipPos.w;
            if (d > depth)
                break;
            seed = (seed * 69723) + 1;

            float distFromVolume = distance(PROP_RAYMARCH_ORIGIN.xz, internalStepPos.xz) - PROP_RAYMARCH_RADIUS;
            float dist = -SAMPLETERRAINMAP(internalStepPos.xz).x;
            if (distFromVolume > 0.0f)
                break;

            float terrainHeight = SAMPLETERRAINMAP(internalStepPos.xz).y;
            float heightDist = abs(internalStepPos.y - terrainHeight);
            float heightFalloff = clamp(1.0f - (heightDist / 300.0f), 0.0f, 1.0f);
            heightFalloff *= heightFalloff;
            if (dist < 0.25f)
                stepsInside += heightFalloff;
            internalStepPos += rayDirection * 2.0f;
        }
    }

    gl_FragColor = vec4(
        1.0f,
        0.95f,
        0.65f,
        stepsInside * 0.0015f
    );
}
