$input  a_position, a_normal, a_tangent, a_bitangent, a_texcoord0, i_data0, i_data1, i_data2, i_data3, i_data4 
$output v_wposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>

uniform vec4 u_normalMult;

void main() {
    mat4 modelMatrix = mtxFromCols(i_data0, i_data1, i_data2, i_data3);

    v_normal = vec3(mul(modelMatrix, vec4(a_normal, 0.0f))); 
    v_normal = normalize(v_normal) * u_normalMult.x;

    v_bitangent = vec3(mul(modelMatrix, vec4(a_bitangent, 0.0f))); 
    v_bitangent = normalize(v_bitangent);

    v_tangent = vec3(mul(modelMatrix, vec4(v_tangent, 0.0f))); 
    v_tangent = normalize(v_tangent);
    v_tbn = mat3(v_tangent, v_bitangent, v_normal);

    v_texcoord0 = a_texcoord0;
    v_wposition = mul(modelMatrix, vec4(a_position, 1.0f));
    v_color = i_data4;
    gl_Position = mul(u_viewProj, vec4(v_wposition, 1.0f));
}
