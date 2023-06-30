$input a_position, a_texcoord0
$output v_texcoord0
#include <bgfx_shader.sh>

static const float WIDTH = 500;
static const float HEIGHT = 32;
static const float MARGIN = 10;

void main() {
    float invTexel = u_viewTexel.x / u_viewTexel.y;

    a_position.x *= 2.0f * WIDTH * u_viewTexel.x;
    a_position.y *= 2.0f * HEIGHT * u_viewTexel.y;
    a_position.x += 1.0f - WIDTH * u_viewTexel.x - MARGIN * u_viewTexel.x;
    a_position.y -= 1.0f - HEIGHT * u_viewTexel.y - MARGIN * u_viewTexel.y;

    v_texcoord0 = a_texcoord0;
    gl_Position = vec4(a_position.x, a_position.y, 0.0f, 1.0f);
}
