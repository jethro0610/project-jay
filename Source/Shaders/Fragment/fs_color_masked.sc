$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#define SHARED_SHADER
#include <Shared_MProps.h>
#include <dither.sh>

SAMPLER2D(s_sampler0, 0);

void main() {
    float alpha = texture2D(s_sampler0, v_texcoord0).a;
    DITHERDISCARD(1.0f - alpha);

    vec3 color = vec3(u_mProps[MPROP_R], u_mProps[MPROP_G], u_mProps[MPROP_B]);
    gl_FragColor = vec4(color, 1.0f);
}
