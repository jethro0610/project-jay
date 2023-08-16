$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#define NOFRESNEL
#include <Lighting.sh>
#include <Crack.sh>

SAMPLER2D(s_color, 0);
SAMPLER2D(s_normal, 1);
SAMPLER2D(s_crack, 2);

void main() {
    vec3 color = texture2D(s_color, v_texcoord0);

    vec3 normal = texture2D(s_normal, v_texcoord0).rgb;
    normal = normal * 2.0f - 1.0f;
    normal = normalize(mul(normal, v_tbn));

    float brightness = getBrightness(normal, v_wposition, v_sposition);

    vec4 crackColor = texture2DLod(s_crack, v_texcoord0, 0);
    vec2 crackStrength = getCrackStrength();
    float crackDepth = crackColor.r * crackStrength.x + crackColor.g * crackStrength.y;
    color = lerp(color, 0.15f, crackDepth);

    gl_FragColor = vec4(color * (brightness), 1.0f);
}
