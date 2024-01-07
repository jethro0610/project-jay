$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <lighting.sh>
#define SHARED_SHADER
#include <Shared_MProps.h>

void main() {
    vec3 color = vec3(u_mProps[MPROP_R], u_mProps[MPROP_G], u_mProps[MPROP_B]);
    vec3 normal = v_normal;

    float brightness = getBrightness(normal, v_wposition, v_sposition);

    gl_FragColor = vec4(color * brightness, 1.0f);
}
