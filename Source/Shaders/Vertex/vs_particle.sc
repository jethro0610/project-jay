$input a_position, a_texcoord0, i_data0, i_data1, i_data2
$output v_sposition, v_texcoord0
#include <bgfx_shader.sh>

#define i_position i_data0.xyz
#define i_velocity vec3(i_data0.w, i_data1.xy)
#define i_rotation i_data1.z
#define i_initialScale i_data1.w
#define i_scale i_data2.x
#define i_time i_data2.y

uniform mat4 u_shadowMatrix;
uniform vec4 u_cameraUp;
uniform vec4 u_cameraRight;

void main() {
    v_texcoord0 = a_texcoord0;
    a_position *= i_scale;
    vec3 position = u_cameraRight.xyz * a_position.x + u_cameraUp.xyz * a_position.y + i_position;
    v_sposition = mul(u_shadowMatrix, vec4(position, 1.0f));
    gl_Position = mul(u_viewProj, vec4(position, 1.0f));
}