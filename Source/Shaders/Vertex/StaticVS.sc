$input a_position, a_normal, a_tangent, a_bitangent, a_texcoord0
$output v_wposition, v_normal, v_tangent, v_bitangent, v_texcoord0
#include <bgfx_shader.sh>

uniform mat3 u_normal;

void main() {
    v_normal = mul(u_normal, a_normal); 
    v_normal = normalize(v_normal);

    v_texcoord0 = a_texcoord0;
    v_wposition = mul(u_model[0], vec4(a_position, 1.0f));
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0f));
}
