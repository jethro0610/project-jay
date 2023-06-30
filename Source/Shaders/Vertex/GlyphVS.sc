$input a_position, a_texcoord0, i_data0
$output v_texcoord0
#include <bgfx_shader.sh>

static const int MAP_CHARS = 16;
static const float CHAR_SIZE = 1.0f / float(MAP_CHARS);

void main() {
    float invTexel = u_viewTexel.x / u_viewTexel.y;

    float x = i_data0.x; 
    float y = i_data0.y; 
    float scale = i_data0.z;

    x *= u_viewTexel.x;
    y *= u_viewTexel.y;
    scale *= u_viewTexel.y;

    a_position.x *= scale * invTexel;
    a_position.y *= scale;
    a_position.x -= 1.0f - scale * invTexel * 0.5f - x;
    a_position.y += 1.0f - scale * 0.5f - y;

    gl_Position = vec4(a_position.x, a_position.y, 0.0f, 1.0f);

    int char = (int)i_data0.w;
    int charRow = floor(char / MAP_CHARS);
    int charCol = char % MAP_CHARS;

    v_texcoord0 = a_texcoord0 * CHAR_SIZE;
    v_texcoord0.x += CHAR_SIZE * charCol; 
    v_texcoord0.y += CHAR_SIZE * charRow; 
}
