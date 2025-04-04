$input v_sposition, v_texcoord0, v_time
#include <bgfx_shader.sh>
#include <shadow.sh>
#include <particle.sh>
#include <dither.sh>

void main() {
    vec2 origin = (0.5f, 0.5f);

    vec4 color = lerp(PART_STARTCOL, PART_ENDCOL, v_time);
    color.a *= 0.5f;
    if (distance(origin, v_texcoord0) > 0.5)
        color.a = 0.0f;

    gl_FragColor = color;
}
