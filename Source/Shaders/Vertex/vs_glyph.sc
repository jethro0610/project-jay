$input a_position, a_texcoord0, i_data0
$output v_texcoord0
#include <bgfx_shader.sh>
#include <text.sh>

static const int MAP_CHARS = 16;
static const float CHAR_SIZE = 1.0f / float(MAP_CHARS);

void main() {
    int index = (int)i_data0.x;
    int character = (int)i_data0.y;

    a_position.x *= 2.0f * TEXT_SCALE * u_viewTexel.x; 
    a_position.y *= 2.0f * TEXT_SCALE * u_viewTexel.y; 
    a_position.x -= 1.0f - TEXT_SCALE * u_viewTexel.x;
    a_position.y += 1.0f - TEXT_SCALE * u_viewTexel.y;

    a_position.x += TEXT_POSITION.x + index * TEXT_SCALE * TEXT_KERNING * u_viewTexel.x * 2.0f;
    a_position.y -= TEXT_POSITION.y * u_viewTexel.y * 2.0f;

    gl_Position = vec4(a_position.x, a_position.y, 0.0f, 1.0f);

    int charRow = floor(character / MAP_CHARS);
    int charCol = character % MAP_CHARS;

    v_texcoord0 = a_texcoord0 * CHAR_SIZE;
    v_texcoord0.x += charCol * CHAR_SIZE; 
    v_texcoord0.y += charRow * CHAR_SIZE; 
}
