$input a_position, a_normal, a_tangent, a_bitangent, a_texcoord0, i_data0, i_data1, i_data2, i_data3, i_data4 
$output v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>

uniform vec4 u_normalMult;
uniform mat4 u_shadowMatrix;
uniform vec4 u_time;

#define i_color i_data4.xyz
#define i_time i_data4.w

static const float TIME_FOR_MAX_SIZE = 0.25f;
static const float c1 = 4.0f;
static const float c3 = c1 + 1.0f;

#define SQR(factor) ((factor) * (factor))
#define CUBE(factor) ((factor) * (factor) * (factor))

void main() {
    mat4 modelMatrix = mtxFromCols(i_data0, i_data1, i_data2, i_data3);

    v_normal = mul(modelMatrix, vec4(a_normal, 0.0f)).xyz; 
    v_normal = normalize(v_normal) * u_normalMult.x;

    v_bitangent = mul(modelMatrix, vec4(a_bitangent, 0.0f)).xyz; 
    v_bitangent = normalize(v_bitangent) * u_normalMult.x;

    v_tangent = mul(modelMatrix, vec4(a_tangent, 0.0f)).xyz; 
    v_tangent = normalize(v_tangent) * u_normalMult.x;

    v_tbn = mat3(v_tangent, v_bitangent, v_normal);

    float time = min(1.0f, (u_time.x - i_time) / TIME_FOR_MAX_SIZE);
    time = 1.0f + c3 * CUBE(time - 1.0f) + c1 * SQR(time - 1.0f);

    v_texcoord0 = a_texcoord0;
    v_wposition = mul(modelMatrix, vec4(a_position * time, 1.0f)).xyz;
    v_sposition = mul(u_shadowMatrix, vec4(v_wposition, 1.0f));

    v_color = vec4(i_color, 1.0f);

    gl_Position = mul(u_viewProj, vec4(v_wposition, 1.0f));
}
