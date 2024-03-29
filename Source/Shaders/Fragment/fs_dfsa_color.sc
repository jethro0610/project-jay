$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <lighting.sh>
#include <properties.sh>

void main() {
    vec3 normal = v_normal;
    float brightness = getBrightness(normal, v_wposition, v_sposition);

    gl_FragColor = vec4(PROP_COLOR.xyz * brightness, 1.0f);
}
