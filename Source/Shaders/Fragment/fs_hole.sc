$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <lighting.sh>
#include <properties.sh>
#include <noise.sh>
#define SHARED_SHADER
#include <Shared_TerrainFuncs.h>

uniform vec4 u_cameraForward;
uniform vec4 u_cameraUp;
uniform vec4 u_time;
uniform vec4 u_cameraRight;
SAMPLER2D(s_sampler0, 0);

float project2Dto3D(vec2 a, vec3 b) {
    vec3 a3 = vec3(a.x, 0.0f, a.y);
    return dot(a3, b) / dot(b, b);
}

float distance2(vec2 a, vec2 b) {
    float x = a.x - b.x;
    float y = a.y - b.y;
    return x * x + y * y;
}

void main() {
    float aspect = 16.0f / 9.0f;
    float fov = 70.0f;
    float fov2 = radians(fov) / 2.0f;
    vec2 rayUv = vec2(
        gl_FragCoord.x / 1024,
        gl_FragCoord.y / 576
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

    const vec3 volumetricOrigin = PROP_VOLUMETRIC_ORIGIN;
    const float volumetricRadius = PROP_VOLUMETRIC_RADIUS;
    const float volumetricRadius2 = volumetricRadius * volumetricRadius;

    vec3 origin = u_cameraPosition.xyz;
    float rayDistance = 0.0f;

    // Raymarch into mesh volume first
    for (int i = 0; i < 16; i++) {
        vec3 pos = origin + rayDirection * rayDistance;
        float distFromVolume = distance(volumetricOrigin.xz, pos.xz) - volumetricRadius;
        if (distFromVolume < 0.01f)
            break;

        rayDistance += project2Dto3D(rayDirection2D * distFromVolume, rayDirection);
    }

    // Accumulate in light in the holes of the terrain
    const float minStepDistance = PROP_VOLUMETRIC_STEP_DIST;
    const float lightAccum = PROP_VOLUMETRIC_LIGHT_ACCUM;
    const float falloffHeight = PROP_VOLUMETRIC_FALLOFF_HEIGHT;
    float totalLightAccum = 0.0f;

    fnl_state noise = fnlCreateState(1337);
    noise.noise_type = FNL_NOISE_OPENSIMPLEX2;

    for (int i = 0; i < 512; i++) {
        vec3 startPos = origin + rayDirection * rayDistance;
        vec3 noisePos = startPos;
        float noiseVal = fnlGetNoise3D(noise, noisePos.x, u_time.x * 50.0f, noisePos.z);
        noiseVal = (noiseVal + 1.0f);

        vec2 terrainSample = getTerrainDistance(startPos.xz, 0, 0, false);
        float height = abs(startPos.y - terrainSample.y);
        terrainSample.x += (height / falloffHeight) * 30.0f; // Expand at ends

        // Raymarch step, but enforce a minimum so we don't get
        // stuck in the marching volume
        float stepDistance = max(-terrainSample.x, minStepDistance);

        // Rays higher than the terrain need to fade
        // out with a falloff

        float falloff = clamp(1.0f - (height / falloffHeight), 0.0f, 1.0f);
        falloff *= falloff;

        if (terrainSample.x > 0.0f)
            totalLightAccum += lightAccum * stepDistance * falloff * noiseVal;

        rayDistance += stepDistance;
        vec3 endPos = origin + rayDirection * rayDistance;

        // The depth of the ray MUST be above the depth
        // of the rendered scene
        vec4 clipPos = mul(u_viewProj, vec4(endPos, 1.0f));
        float d = clipPos.z / clipPos.w;
        if (d > depth) {
            // Rays may accumulate past the depth, resulting in
            // inaccurate lights. So, we need to remove some
            // light depending on how deep into the depth the
            // ray is
            // totalLightAccum -= (d - depth) * 0.25f * stepDistance * falloff * noiseVal;
            break;
        }

        // Any rays that exit the volume are complete
        float dist2FromVolume = distance2(volumetricOrigin.xz, endPos.xz);
        if (dist2FromVolume > volumetricRadius2 + 1.0f)
            break;
    }

    vec4 volumeColor = PROP_VOLUMETRIC_COLOR;
    gl_FragColor = vec4(
        volumeColor.r,
        volumeColor.g,
        volumeColor.b,
        totalLightAccum * volumeColor.a
    );
}
