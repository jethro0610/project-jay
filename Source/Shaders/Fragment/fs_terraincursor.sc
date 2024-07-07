$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <dither.sh>

void main() {
    float uvDist = distance(v_texcoord0, vec2(0.5f, 0.5f));
    float opacity = (1.0f - uvDist / 0.5f);
    opacity = clamp(opacity, 0.0f, 1.0f);
    DITHEROPACITY(opacity);
    gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
