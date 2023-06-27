$input v_wposition, v_normal
#include <bgfx_shader.sh>

SAMPLER2D(s_sampler0, 0);

void main() {
    float val = texture2D(s_sampler0, vec2(v_wposition.x / 64.0f, v_wposition.z / 64.0f));
    gl_FragColor = val;
}
