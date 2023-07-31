$input a_position, a_normal, a_tangent, a_bitangent, a_texcoord0, i_data0, i_data1, i_data2, i_data3
$output v_wposition, v_normal, v_tangent, v_bitangent, v_tbn, v_texcoord0
#include <bgfx_shader.sh>

// uniform mat3 u_normal;
uniform vec4 u_normalMult;

void main() {
    mat4 model = mtxFromCols(i_data0, i_data1, i_data2, i_data3);

    v_normal = a_normal * u_normalMult.x; 
    v_bitangent = a_bitangent;
    v_tangent = a_tangent;
    v_tbn = mat3(v_tangent, v_bitangent, v_normal);

    v_texcoord0 = a_texcoord0;
    v_wposition = mul(model, vec4(a_position, 1.0f));
    gl_Position = mul(u_viewProj, vec4(v_wposition, 1.0f));
}
