$input a_position, i_data0, i_data1, i_data2, i_data3
#include <bgfx_shader.sh>

// Change this to shadow
uniform vec4 u_cameraUp;
uniform vec4 u_cameraRight;

void main() {
    a_position *= 0.35f;
    vec3 position = u_cameraRight.xyz * a_position.x + u_cameraUp.xyz * a_position.y + i_data0.xyz;
    gl_Position = mul(u_viewProj, vec4(position, 1.0f));
}
