$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <Shared_Properties.sh>
#include <dither.sh>

SAMPLER2D(s_sampler0, 0);

void main() {
    float alpha = texture2D(s_sampler0, v_texcoord0).a;
    DITHERDISCARD(1.0f - alpha);

    vec3 color = vec3(u_mProp[MPROP_R], u_mProp[MPROP_G], u_mProp[MPROP_B]);
    gl_FragColor = vec4(color, 1.0f);
}
