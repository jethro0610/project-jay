#include "../FragTypes/ScreenFrag.hlsli"
#include "../VertTypes/TextVert.hlsli"
#include "../Constants/ConstantBuffers.hlsli"

static const int CHARS_PER_LINE = 32;
static const int MAP_CHARS = 16;
static const float CHAR_SIZE = 1.0f / float(MAP_CHARS);
static const float KERNING = 0.5f;
static const float HEIGHT = 0.125f;
static const float KERN_WIDTH = 1.0f - KERNING;
static const float MARGIN = 0.05f;

ScreenFrag main(TextVert vert) {
    ScreenFrag frag;
    int curLine = floor(vert.instanceID / CHARS_PER_LINE);
    int curChar = vert.instanceID % CHARS_PER_LINE;

    float inverseRatio = 1.0f / aspectRatio;
    float marginX = MARGIN * inverseRatio;

    float width = KERN_WIDTH * HEIGHT * inverseRatio;

    float2 coord = float2(vert.vertexID&1, vert.vertexID>>1);
    float originX = coord.x * width - 1.0f;
    float originY = -coord.y * HEIGHT + 1.0f;
    float posX = originX + width * curChar + marginX;
    float posY = originY - HEIGHT * curLine - MARGIN;

    frag.uv = coord * CHAR_SIZE;
    frag.uv .x *= KERN_WIDTH;
    int charRow = floor(vert.charIndex / MAP_CHARS);
    int charCol = vert.charIndex % MAP_CHARS;

    frag.uv.x += CHAR_SIZE * charCol; 
    frag.uv.y += CHAR_SIZE * charRow; 

    frag.position = float4(posX, posY, 0.0f, 1.0f);
    return frag;
}
