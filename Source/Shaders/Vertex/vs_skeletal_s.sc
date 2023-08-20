$input a_position, a_texcoord0, a_joints, a_weights
$output v_texcoord0
#include <bgfx_shader.sh>

void main() {
    v_texcoord0 = a_texcoord0;
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0f));
}
