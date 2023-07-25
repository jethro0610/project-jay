$input a_position, a_normal, a_tangent, a_bitangent, a_texcoord0, i_data0
$output v_wposition, v_vposition, v_normal, v_tangent, v_bitangent, v_tbn, v_texcoord0
#include <bgfx_shader.sh>

uniform mat3 u_normal;

void main() {
    v_normal = a_normal; 
    v_bitangent = a_bitangent;
    v_tangent = a_tangent;
    v_tbn = mat3(v_tangent, v_bitangent, v_normal);

    v_texcoord0 = a_texcoord0;
    v_wposition = i_data0.xyz + a_position;
    v_vposition = a_position;
    gl_Position = mul(u_viewProj, vec4(v_wposition, 1.0f));
}
