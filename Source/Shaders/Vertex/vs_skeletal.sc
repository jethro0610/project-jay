$input a_position, a_normal, a_tangent, a_bitangent, a_texcoord0, a_color0, a_color1 
$output v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <properties.sh>

uniform vec4 u_normalMult;
uniform mat4 u_shadowMatrix;
uniform mat4 u_pose[32];

static const mat4 identity = {
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f}
};

void main() {

    mat4 skinMatrix = 
        a_color1.x * u_pose[a_color0.x] +
        a_color1.y * u_pose[a_color0.y] +
        a_color1.z * u_pose[a_color0.z] +
        a_color1.w * u_pose[a_color0.w];

    v_normal = mul(identity, mul(skinMatrix, vec4(a_normal, 0.0f))).xyz; 
    v_normal = normalize(v_normal) * u_normalMult.x;

    v_bitangent = mul(identity, mul(skinMatrix, vec4(a_bitangent, 0.0f))).xyz; 
    v_bitangent = normalize(v_bitangent) * u_normalMult.x;

    v_tangent = mul(identity, mul(skinMatrix, vec4(a_tangent, 0.0f))).xyz; 
    v_tangent = normalize(v_tangent) * u_normalMult.x;

    v_tbn = mat3(v_tangent, v_bitangent, v_normal);

    v_texcoord0 = a_texcoord0;
    v_wposition = mul(identity, mul(skinMatrix, vec4(a_position, 1.0f))).xyz;
    v_sposition = mul(u_shadowMatrix, vec4(v_wposition, 1.0f));

    gl_Position = mul(u_viewProj, mul(skinMatrix, vec4(a_position, 1.0f)));
}
