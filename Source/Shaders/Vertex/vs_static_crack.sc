$input a_position, a_normal, a_tangent, a_bitangent, a_texcoord0
$output v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <Crack.sh>

SAMPLER2D(s_crack, 2);
uniform mat4 u_shadowMatrix;

void main() {
    v_normal = mul(u_model[0], vec4(a_normal, 0.0f)).xyz; 
    v_normal = normalize(v_normal);

    v_bitangent = mul(u_model[0], vec4(a_bitangent, 0.0f)).xyz; 
    v_bitangent = normalize(v_bitangent);

    v_tangent = mul(u_model[0], vec4(a_tangent, 0.0f)).xyz; 
    v_tangent = normalize(v_tangent);

    v_tbn = mat3(v_tangent, v_bitangent, v_normal);

    vec4 crackColor = texture2DLod(s_crack, a_texcoord0, 0);
    vec2 crackStrength = getCrackStrength();
    float crackDepth = crackColor.r * crackStrength.x + crackColor.g * crackStrength.y;
    a_position += a_normal * pow(crackDepth, 0.25f) * -0.1f;

    v_texcoord0 = a_texcoord0;

    v_wposition = mul(u_model[0], vec4(a_position, 1.0f)).xyz;
    v_sposition = mul(u_shadowMatrix, vec4(v_wposition, 1.0f));
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0f));
}
