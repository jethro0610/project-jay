$input a_position, i_data0, i_data1, i_data2, i_data3
#include <bgfx_shader.sh>

void main() {
    mat4 modelMatrix = mtxFromCols(i_data0, i_data1, i_data2, i_data3);
    vec3 position = mul(modelMatrix, vec4(a_position, 1.0f));
    gl_Position = mul(u_viewProj, vec4(position, 1.0f));
}
