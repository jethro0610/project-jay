$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <lighting.sh>
#include <dither.sh>

SAMPLER2D(s_sampler0, 0);
static const vec3 SELECTION_COLOR = vec3(1.0f, 1.0f, 1.0f);

void main() {
    DITHERDISCARD(0.5f);
    float brightness = getBrightness(v_normal, v_wposition, v_sposition);
    gl_FragColor = vec4(SELECTION_COLOR * brightness, 1.0f);
}
