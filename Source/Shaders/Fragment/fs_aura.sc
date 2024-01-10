$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <properties.sh>
#include <dither.sh>
#include <noise.sh>

#define NOSPECULAR
#include <lighting.sh>

void main() {
    DITHERDISCARD(PROP_COLOR.w);
    vec3 normal = v_normal;
    float brightness = getBrightness(normal, v_wposition, v_sposition);

    fnl_state noise = fnlCreateState(1337);
    noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
    vec3 variationPos = v_wposition * 6.0f;
    float noiseVal = fnlGetNoise3D(noise, variationPos.x, variationPos.y, variationPos.z);
    noiseVal = (noiseVal + 1.0f) * 0.5f;
    noiseVal = lerp(0.65f, 1.0f, pow(noiseVal, 4.0f));

    gl_FragColor = vec4(brightness * noiseVal * PROP_COLOR.xyz, 1.0f);
}
