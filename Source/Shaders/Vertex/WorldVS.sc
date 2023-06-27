$input a_position, a_normal
$output v_wposition, v_normal
#include <bgfx_shader.sh>

SAMPLER2D(s_sampler0, 0);

void main() {
    // float val = texture2D(s_sampler0, vec2(a_position.x / 64.0f, a_position.z / 64.0f));
    vec2 samplePos = vec2(a_position.x / 64.0f, a_position.z / 64.0f);
    float val = texture2DLod(s_sampler0, samplePos, 0);


    // v_wposition = vec3(a_position.x, 8.0f * (1.0f - test), a_position.z);
    v_wposition = vec3(a_position.x, 12.0f * (1.0f - val), a_position.z);

    gl_Position = mul(u_viewProj, vec4(v_wposition, 1.0f));
}
