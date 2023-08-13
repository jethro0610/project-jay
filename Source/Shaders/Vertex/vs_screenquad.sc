$input a_position, a_texcoord0
$output v_texcoord0
#include <bgfx_shader.sh>

void main() {
    v_texcoord0 = a_texcoord0;
    a_position *= 2.0f;
    gl_Position = vec4(a_position.x, a_position.y, 0.0f, 1.0f);
}
