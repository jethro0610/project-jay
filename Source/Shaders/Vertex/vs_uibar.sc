$input a_position, a_texcoord0
$output v_texcoord0
#include <bgfx_shader.sh>
#include <ui.sh>

void main() {
    float invTexel = u_viewTexel.x / u_viewTexel.y;

    // Setup the corners of the glyph
    a_position.x *= UI_ELEM_SIZE_X * u_viewTexel.x * 2.0f; 
    a_position.y *= UI_ELEM_SIZE_Y * u_viewTexel.y * 2.0f; 
    a_position.x -= 1.0f - UI_ELEM_SIZE_X * u_viewTexel.x;
    a_position.y += 1.0f - UI_ELEM_SIZE_Y * u_viewTexel.y;

    float hAlignOffset = UI_ELEM_SIZE_X * UI_ELEM_H_ALIGN;
    float vAlignOffset = UI_ELEM_SIZE_Y * UI_ELEM_V_ALIGN;
    float hAnchorOffset = (1.0f / u_viewTexel.x) * UI_ELEM_H_ANCHOR;
    float vAnchorOffset = (1.0f / u_viewTexel.y) * UI_ELEM_V_ANCHOR;

    a_position.x += (hAnchorOffset + UI_ELEM_POSITION_X - hAlignOffset) * u_viewTexel.x * 2.0f;
    a_position.y -= (vAnchorOffset + UI_ELEM_POSITION_Y - vAlignOffset) * u_viewTexel.y * 2.0f;

    v_texcoord0 = a_texcoord0;
    gl_Position = vec4(a_position.x, a_position.y, 0.0f, 1.0f);
}
