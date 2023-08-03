$input a_position, a_texcoord0, i_data0
$output v_sposition, v_texcoord0
#include <bgfx_shader.sh>

uniform mat4 u_shadowMatrix;
uniform vec4 u_cameraUp;
uniform vec4 u_cameraRight;

void main() {
    v_texcoord0 = a_texcoord0;
    a_position *= 0.45f;
    vec3 position = u_cameraRight.xyz * a_position.x + u_cameraUp.xyz * a_position.y + i_data0.xyz;
    v_sposition = mul(u_shadowMatrix, vec4(position, 1.0f));
    gl_Position = mul(u_viewProj, vec4(position, 1.0f));
}
