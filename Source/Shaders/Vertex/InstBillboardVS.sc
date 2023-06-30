$input a_position, a_texcoord0, i_data0
$output v_texcoord0
#include <bgfx_shader.sh>

uniform vec4 u_cameraUp;
uniform vec4 u_cameraRight;

void main() {
    v_texcoord0 = a_texcoord0;
    a_position *= 0.5f;
    vec3 position = u_cameraRight.xyz * a_position.x + u_cameraUp.xyz * a_position.y + i_data0.xyz;
    gl_Position = mul(u_viewProj, vec4(position, 1.0f));
}
