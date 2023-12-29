$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <lighting.sh>
#include <noise.sh>

SAMPLER2D(s_sampler0, 0);
SAMPLER2D(s_sampler1, 1);

void main() {
    vec3 color = texture2D(s_sampler0, v_texcoord0).rgb;

    vec3 normal = texture2D(s_sampler1, v_texcoord0).rgb;
    normal = normal * 2.0f - 1.0f;
    normal = normalize(mul(normal, v_tbn));

    float brightness = getBrightness(normal, v_wposition, v_sposition);

    fnl_state noise = fnlCreateState(1337);
    noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
    float noiseVal = (fnlGetNoise3D(noise, v_wposition.x * 4.0f, v_wposition.y * 4.0f, v_wposition.z * 4.0f) + 1.0f) * 0.5f;
    noiseVal = lerp(0.25f, 1.25f, pow(noiseVal, 0.5f));

    gl_FragColor = vec4(color * brightness * noiseVal, 1.0f);
}
