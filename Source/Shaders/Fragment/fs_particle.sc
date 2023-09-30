$input v_sposition, v_texcoord0, v_time
#include <bgfx_shader.sh>
#include <shadow.sh>
#include <particle.sh>
#include <dither.sh>

void main() {
    vec2 origin = (0.5f, 0.5f);
    if (distance(origin, v_texcoord0) > 0.5)
        discard;

    vec4 color = lerp(PART_STARTCOL, PART_ENDCOL, v_time);
    DITHERDISCARD(1.0f - color.w);
    gl_FragColor = color;
}
