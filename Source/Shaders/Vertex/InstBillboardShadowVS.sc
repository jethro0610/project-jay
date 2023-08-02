$input a_position, a_texcoord0, i_data0, i_data1, i_data2, i_data3
$output v_texcoord0
#include <bgfx_shader.sh>

uniform vec4 u_shadowUp;
uniform vec4 u_shadowRight;

void main() {
    v_texcoord0 = a_texcoord0;
    a_position *= 0.35f;
    vec3 position = u_shadowRight.xyz * a_position.x + u_shadowUp.xyz * a_position.y + i_data0.xyz;
    gl_Position = mul(u_viewProj, vec4(position, 1.0f));
}
