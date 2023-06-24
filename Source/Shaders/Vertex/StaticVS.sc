$input a_position, a_normal, a_tangent, a_bitangent, a_texcoord0
$output v_wpos, v_normal, v_tangent, v_bitangent, v_texcoord0
#include <bgfx_shader.sh>

uniform mat4 u_normal;

void main() {
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0f));
    v_wpos = mul(u_model[0], vec4(a_position, 1.0f)).xyz;
    
    vec4 normal = vec4(a_normal.xyz, 0.0f);
    v_normal = mul(u_normal, normal).xyz;
    v_normal = normalize(v_normal);

    vec4 tangent = vec4(a_tangent.xyz, 0.0f);
    v_tangent = mul(u_normal, tangent).xyz;
    v_tangent = normalize(v_tangent);

    vec4 bitangent = vec4(a_bitangent.xyz, 0.0f);
    v_bitangent = mul(u_normal, bitangent).xyz;
    v_bitangent = normalize(v_bitangent);

    v_texcoord0 = a_texcoord0;
}
