$input a_position, a_texcoord0, i_data0
$output v_texcoord0
#include <bgfx_shader.sh>
#include <worldtext.sh>

static const int MAP_CHARS = 16;
static const float CHAR_SIZE = 1.0f / float(MAP_CHARS);
static const float KERNING = 0.45;

uniform vec4 u_cameraUp;
uniform vec4 u_cameraRight;

void main() {
    int index = (int)i_data0.x;
    int character = (int)i_data0.y;

    // Assign the textture coordinates for the character
    int charRow = floor(character / MAP_CHARS);
    int charCol = character % MAP_CHARS;

    v_texcoord0 = a_texcoord0 * CHAR_SIZE;
    v_texcoord0.x += charCol * CHAR_SIZE; 
    v_texcoord0.y += charRow * CHAR_SIZE; 
    a_position *= WORLD_TEXT_SIZE;
    vec3 position = u_cameraRight.xyz * a_position.x + u_cameraUp.xyz * a_position.y + WORLD_TEXT_POSITION;

    float hTextLength = WORLD_TEXT_SIZE * KERNING * WORLD_TEXT_COUNT;
    position += (u_cameraRight.xyz * WORLD_TEXT_SIZE * index * KERNING);
    position -= (u_cameraRight.xyz * hTextLength * WORLD_TEXT_H_ALIGN);
    position += (u_cameraRight.xyz * WORLD_TEXT_SIZE * WORLD_TEXT_H_ALIGN);

    gl_Position = mul(u_viewProj, vec4(position, 1.0f));
}
