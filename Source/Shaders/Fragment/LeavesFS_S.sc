$input v_texcoord0
#include <bgfx_shader.sh>

SAMPLER2D(s_sampler0, 0);

void main() {
    float alpha = texture2D(s_sampler0, v_texcoord0).a;
    if (alpha < 0.15f)
        discard;
}
