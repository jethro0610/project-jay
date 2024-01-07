$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <lighting.sh>
#include <Shared_Properties.sh>
#include <noise.sh>

SAMPLER2D(s_sampler0, 0);
SAMPLER2D(s_sampler1, 1);

void main() {
    vec2 coords = vec2(v_texcoord0.x * PROP_TEXSCALE_X, v_texcoord0.y * PROP_TEXSCALE_Y);
    vec3 color = texture2D(s_sampler0, coords).rgb;

    vec3 normal = texture2D(s_sampler1, coords).rgb;
    normal = normal * 2.0f - 1.0f;
    normal = normalize(mul(normal, v_tbn));

    float brightness = getBrightness(normal, v_wposition, v_sposition);

    fnl_state noise = fnlCreateState(1337);
    noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
    vec3 variationPos = v_wposition * u_mProps[MPROP_VARIATION_FREQUENCY];
    float noiseVal = fnlGetNoise3D(noise, variationPos.x, variationPos.y, variationPos.z);
    noiseVal = (noiseVal + 1.0f) * 0.5f;
    noiseVal = lerp(u_mProps[MPROP_VARIATION_MIN], u_mProps[PROP_VARIATION_MAX], pow(noiseVal, u_MProps[PROP_VARIATION_POWER]));

    gl_FragColor = vec4(color * brightness * noiseVal, 1.0f);
}
