#include "TextOut.hlsli"
#include "TextIn.hlsli"
#include "ConstantBuffers.hlsli"

static const int CHARS_PER_LINE = 32;
static const int MAP_CHARS = 16;
static const float CHAR_SIZE = 1.0f / float(MAP_CHARS);
static const float KERNING = 0.5f;
static const float HEIGHT = 0.125f;
static const float KERN_WIDTH = 1.0f - KERNING;
static const float MARGIN = 0.05f;

TextOut main(TextIn inputText) {
    TextOut output;
    int curLine = floor(inputText.instanceID / CHARS_PER_LINE);
    int curChar = inputText.instanceID % CHARS_PER_LINE;

    float inverseRatio = 1.0f / aspectRatio;
    float marginX = MARGIN * inverseRatio;

    float width = KERN_WIDTH * HEIGHT * inverseRatio;

    float2 coord = float2(inputText.vertexID&1, inputText.vertexID>>1);
    float originX = coord.x * width - 1.0f;
    float originY = -coord.y * HEIGHT + 1.0f;
    float posX = originX + width * curChar + marginX;
    float posY = originY - HEIGHT * curLine - MARGIN;

    output.coord = coord * CHAR_SIZE;
    output.coord.x *= KERN_WIDTH;
    int charRow = floor(inputText.charIndex / MAP_CHARS);
    int charCol = inputText.charIndex % MAP_CHARS;

    output.coord.x += CHAR_SIZE * charCol; 
    output.coord.y += CHAR_SIZE * charRow; 

    output.pos = float4(posX, posY, 0.0f, 1.0f);
    return output;
}
