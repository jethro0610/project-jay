$input v_texcoord0
#include <bgfx_shader.sh>

SAMPLER2D(s_sampler0, 0);

void main() {
    vec2 origin = (0.5f, 0.5f);
    if (distance(origin, v_texcoord0) > 0.5)
        discard;
}
