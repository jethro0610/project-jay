$input a_position, a_normal, a_tangent, a_bitangent, a_texcoord0
$output v_wposition, v_vposition, v_normal, v_tangent, v_bitangent, v_tbn, v_texcoord0
#include <bgfx_shader.sh>

uniform mat3 u_normal;
uniform vec4 u_normalMult;

void main() {
    v_normal = mul(u_normal, a_normal); 
    v_normal = normalize(v_normal) * u_normalMult.x;

    v_bitangent = mul(u_normal, a_bitangent);
    v_bitangent = normalize(v_bitangent);

    v_tangent = mul(u_normal, a_tangent);
    v_tangent = normalize(v_tangent);

    v_tbn = mat3(v_tangent, v_bitangent, v_normal);

    v_texcoord0 = a_texcoord0;
    v_wposition = mul(u_model[0], vec4(a_position, 1.0f));
    v_vposition = a_position;
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0f));
}
