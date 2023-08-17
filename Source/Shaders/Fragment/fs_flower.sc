$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#define NOSPECULAR
#define NOFRESNEL
#include <lighting.sh>
#include <dither.sh>

SAMPLER2D(s_sampler0, 0);

void main() {
    vec4 texColor = texture2D(s_sampler0, v_texcoord0); 
    float alpha = texColor.a;
    DITHERDISCARD(1.0f - alpha);

    vec3 color = v_color * texColor.r;
    vec3 normal = v_normal;

    float brightness = getBrightness(normal, v_wposition, v_sposition);
    float edge = 1.0f - alpha; 

    gl_FragColor = vec4(color * (brightness + edge), 1.0f);
}
