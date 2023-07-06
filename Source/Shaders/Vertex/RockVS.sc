$input a_position, a_normal, a_tangent, a_bitangent, a_texcoord0
$output v_wposition, v_normal, v_tangent, v_bitangent, v_tbn, v_texcoord0
#include <bgfx_shader.sh>

uniform mat3 u_normal;

SAMPLER2D(s_sampler2, 2);

void main() {
    v_normal = mul(u_normal, a_normal); 
    v_normal = normalize(v_normal);

    v_bitangent = mul(u_normal, a_bitangent);
    v_bitangent = normalize(v_bitangent);

    v_tangent = mul(u_normal, a_tangent);
    v_tangent = normalize(v_tangent);

    v_tbn = mat3(v_tangent, v_bitangent, v_normal);

    vec4 crackColor = texture2DLod(s_sampler2, a_texcoord0, 0);
    float crackDepth = crackColor.r + crackColor.g;
    crackDepth = 0.0f;
    a_position += a_normal * pow(crackDepth, 0.25f) * -0.1f;

    v_texcoord0 = a_texcoord0;

    v_wposition = mul(u_model[0], vec4(a_position, 1.0f));
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0f));
}
