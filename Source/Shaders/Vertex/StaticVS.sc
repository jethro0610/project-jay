$input a_position, a_normal, a_tangent, a_bitangent, a_texcoord0
$output v_wposition, v_normal, v_tangent, v_bitangent, v_tbn, v_texcoord0
#include <bgfx_shader.sh>

uniform vec4 u_normalMult;

void main() {
    v_normal = vec3(mul(u_model[0], vec4(a_normal, 0.0f))); 
    v_normal = normalize(v_normal);

    v_bitangent = vec3(mul(u_model[0], vec4(a_bitangent, 0.0f))); 
    v_bitangent = normalize(v_bitangent);

    v_tangent = vec3(mul(u_model[0], vec4(a_tangent, 0.0f))); 
    v_tangent = normalize(v_tangent);

    v_tbn = mat3(v_tangent, v_bitangent, v_normal);

    v_texcoord0 = a_texcoord0;
    v_wposition = mul(u_model[0], vec4(a_position, 1.0f));
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0f));
}
