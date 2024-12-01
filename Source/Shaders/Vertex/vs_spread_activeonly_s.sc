$input a_position, a_normal, a_tangent, a_bitangent, a_texcoord0, i_data0, i_data1, i_data2, i_data3, i_data4
$output v_texcoord0
#include <bgfx_shader.sh>

uniform vec4 u_time;

#define i_time i_data4.x
#define i_active (i_data4.y > 0.0f)

static const float TIME_FOR_MAX_SIZE = 0.25f;
static const float c1 = 4.0f;
static const float c3 = c1 + 1.0f;

#define SQR(factor) ((factor) * (factor))
#define CUBE(factor) ((factor) * (factor) * (factor))

void main() {
    mat4 modelMatrix = mtxFromCols(i_data0, i_data1, i_data2, i_data3);

    float time = min(1.0f, (u_time.x - i_time) / TIME_FOR_MAX_SIZE);
    time = 1.0f + c3 * CUBE(time - 1.0f) + c1 * SQR(time - 1.0f);
    float activeScale = i_active ? 1.0f : 0.0f;

    vec3 position = mul(modelMatrix, vec4(a_position * time * activeScale, 1.0f)).xyz;
    v_texcoord0 = a_texcoord0;

    gl_Position = mul(u_viewProj, vec4(position, 1.0f));
}

