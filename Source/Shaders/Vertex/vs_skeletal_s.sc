$input a_position, a_texcoord0, a_color0, a_color1 
$output v_texcoord0
#include <bgfx_shader.sh>

uniform mat4 u_pose[64];

void main() {
    mat4 skinMatrix = 
        a_color1.x * u_pose[a_color0.x] +
        a_color1.y * u_pose[a_color0.y] +
        a_color1.z * u_pose[a_color0.z] +
        a_color1.w * u_pose[a_color0.w];

    v_texcoord0 = a_texcoord0;
    gl_Position = mul(u_viewProj, mul(skinMatrix, vec4(a_position, 1.0f)));
}
