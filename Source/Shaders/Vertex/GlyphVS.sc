$input a_position, a_texcoord0, i_data0
$output v_texcoord0
#include <bgfx_shader.sh>

static const int MAP_CHARS = 16;
static const float CHAR_SIZE = 1.0f / float(MAP_CHARS);

void main() {
    vec2 scale = i_data0.z;

    a_position.x *= 2.0f * scale * u_viewTexel.x; 
    a_position.y *= 2.0f * scale * u_viewTexel.y; 
    a_position.x -= 1.0f - scale * u_viewTexel.x;
    a_position.y += 1.0f - scale * u_viewTexel.y;

    a_position.x += i_data0.x * u_viewTexel.x * 2.0f;
    a_position.y -= i_data0.y * u_viewTexel.y * 2.0f;

    gl_Position = vec4(a_position.x, a_position.y, 0.0f, 1.0f);

    int character = (int)i_data0.w;
    int charRow = floor(character / MAP_CHARS);
    int charCol = character % MAP_CHARS;

    v_texcoord0 = a_texcoord0 * CHAR_SIZE;
    v_texcoord0.x += charCol * CHAR_SIZE; 
    v_texcoord0.y += charRow * CHAR_SIZE; 
}
