$input v_sposition, v_texcoord0, v_time
#include <bgfx_shader.sh>
#include <shadow.sh>
#include <particle.sh>

void main() {
    gl_FragColor = lerp(PART_STARTCOL, PART_ENDCOL, v_time);
}
