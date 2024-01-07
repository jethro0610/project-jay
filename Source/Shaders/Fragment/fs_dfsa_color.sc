$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <lighting.sh>
#include <Shared_Properties.sh>

void main() {
    vec3 color = vec3(u_mProp[MPROP_R], u_mProp[MPROP_G], u_mProp[MPROP_B]);
    vec3 normal = v_normal;

    float brightness = getBrightness(normal, v_wposition, v_sposition);

    gl_FragColor = vec4(color * brightness, 1.0f);
}
