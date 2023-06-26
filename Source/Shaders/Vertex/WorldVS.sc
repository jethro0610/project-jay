$input a_position, a_normal
$output v_wposition, v_normal
#include <bgfx_shader.sh>

void main() {
    v_wposition = vec3(a_position.x, 16.0f, a_position.z);
    gl_Position = mul(u_viewProj, vec4(v_wposition, 1.0f));
}
