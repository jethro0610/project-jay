$input a_position, a_normal, a_tangent, a_bitangent, a_texcoord0
$output v_worldPosition, v_normal, v_tangent, v_bitangent, v_texcoord
#include <bgfx_shader.sh>

uniform mat4 u_normal;

void main() {
    gl_Position = mul(u_modelViewProj, float4(a_position, 1.0f));
    v_worldPosition = mul(u_model[0], float4(a_position, 1.0f));
    v_normal = normalize(mul(u_normal, a_normal));
    v_tangent = normalize(mul(u_normal, a_tangent));
    v_bitangent = normalize(mul(u_normal, a_bitangent));
    v_texcoord = float2(a_texcoord0.x, a_texcoord0.y);
}
