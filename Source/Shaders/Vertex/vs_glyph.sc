$input a_position, a_texcoord0, i_data0
$output v_texcoord0
#include <bgfx_shader.sh>
#include <text.sh>

static const int MAP_CHARS = 16;
static const float CHAR_SIZE = 1.0f / float(MAP_CHARS);

void main() {
    int index = (int)i_data0.x;
    int character = (int)i_data0.y;

    // Setup the corners of the glyph
    a_position.x *= TEXT_SCALE * u_viewTexel.x * 2.0f; 
    a_position.y *= TEXT_SCALE * u_viewTexel.y * 2.0f; 
    a_position.x -= 1.0f - TEXT_SCALE * u_viewTexel.x;
    a_position.y += 1.0f - TEXT_SCALE * u_viewTexel.y;

    // Position the glyph on the screen
    float hTextLength = TEXT_SCALE * TEXT_KERNING * TEXT_COUNT;
    float hAlignOffset = hTextLength * TEXT_H_ALIGN;
    float vAlignOffset = TEXT_SCALE * TEXT_V_ALIGN;
    float hAnchorOffset = (1.0f / u_viewTexel.x) * TEXT_H_ANCHOR;
    float vAnchorOffset = (1.0f / u_viewTexel.y) * TEXT_V_ANCHOR;

    a_position.x += (hAnchorOffset + TEXT_POSITION_X - hAlignOffset + TEXT_SCALE * TEXT_KERNING * index) * u_viewTexel.x * 2.0f;
    a_position.y -= (vAnchorOffset + TEXT_POSITION_Y - vAlignOffset) * u_viewTexel.y * 2.0f;
    gl_Position = vec4(a_position.x, a_position.y, 0.0f, 1.0f);

    // Assign the textture coordinates for the character
    int charRow = floor(character / MAP_CHARS);
    int charCol = character % MAP_CHARS;

    v_texcoord0 = a_texcoord0 * CHAR_SIZE;
    v_texcoord0.x += charCol * CHAR_SIZE; 
    v_texcoord0.y += charRow * CHAR_SIZE; 
}
