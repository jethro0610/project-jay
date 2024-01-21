$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#define NOSHADOW
#define NOSPECULAR
#include <bgfx_shader.sh>
#include <lighting.sh>
#include <dither.sh>

SAMPLER2D(s_sampler0, 0);
static const vec3 PERSIST_COLOR = vec3(0.25f, 0.25f, 0.25f);

void main() {
    DITHERDISCARD(0.15f);
    float brightness = getBrightness(v_normal, v_wposition, v_sposition);
    gl_FragColor = vec4(PERSIST_COLOR * brightness, 1.0f);
}
