$input a_position, a_texcoord0, i_data0, i_data1, i_data2, i_data3
$output v_sposition, v_texcoord0, v_time
#include <bgfx_shader.sh>
#include <particle.sh>

uniform mat4 u_shadowMatrix;
uniform vec4 u_cameraUp;
uniform vec4 u_cameraRight;

#define i_initialPosition i_data0
#define i_position i_data1
#define i_velocity i_data2
#define i_rotation i_data3.x
#define i_initialScale i_data3.x
#define i_scale i_data3.z
#define i_time i_data3.w

void main() {
    v_time = i_time;
    v_texcoord0 = a_texcoord0;

    float2x2 rotMat = mat2(cos(i_rotation), -sin(i_rotation), sin(i_rotation), cos(i_rotation));
    vec2 vertPos = mul(a_position.xy * i_scale, rotMat);

    vec3 position = u_cameraRight.xyz * vertPos.x + u_cameraUp.xyz * vertPos.y + i_position.xyz;
    v_sposition = mul(u_shadowMatrix, vec4(position, 1.0f));
    gl_Position = mul(u_modelViewProj, vec4(position, 1.0f));
}
