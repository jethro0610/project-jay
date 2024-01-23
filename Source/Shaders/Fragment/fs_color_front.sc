$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <properties.sh>
#include <dither.sh>

void main() {
    DITHERDISCARD(1.0f - PROP_COLOR.w);
    gl_FragColor = vec4(PROP_COLOR.xyz, 1.0f);
    gl_FragDepth = 0.0f;
}
