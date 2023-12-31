$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <lighting.sh>

SAMPLER2D(s_sampler0, 0);
SAMPLER2D(s_sampler1, 1);

void main() {
    vec2 coords = vec2(v_texcoord0.x * PROP_TEXSCALE_X, v_texcoord0.y * PROP_TEXSCALE_Y);
    vec3 color = texture2D(s_sampler0, coords).rgb;

    vec3 normal = texture2D(s_sampler1, coords).rgb;
    normal = normal * 2.0f - 1.0f;
    normal = normalize(mul(normal, v_tbn));

    float brightness = getBrightness(normal, v_wposition, v_sposition);

    gl_FragColor = vec4(color * brightness, 1.0f);
}
