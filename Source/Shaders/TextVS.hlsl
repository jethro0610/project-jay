#include "TextOut.hlsli"
#include "TextIn.hlsli"
#include "ConstantBuffers.hlsli"

static const int CHARS_PER_LINE = 32;
static const int MAP_CHARS = 16;
static const float CHAR_SIZE = 1.0f / float(MAP_CHARS);
static const float KERNING = 0.3f;

TextOut main(TextIn inputText) {
    TextOut output;
    int curLine = floor(inputText.instanceID / CHARS_PER_LINE);
    int curChar = inputText.instanceID % CHARS_PER_LINE;

    float inverseRatio = 1.0f / aspectRatio;
    float marginY = 0.05f;
    float marginX = marginY * inverseRatio;

    float height = 0.075f;
    float width = height * inverseRatio;

    float2 coord = float2(inputText.vertexID&1, inputText.vertexID>>1);
    float originX = coord.x * width - 1.0f;
    float originY = -coord.y * height + 1.0f;
    float posX = originX + (width - KERNING * height) * curChar + marginX;
    float posY = originY - height * curLine - marginY;

    output.coord = coord * CHAR_SIZE;
    /* output.coord.x += CHAR_SIZE; */
    int charRow = floor(inputText.charIndex / MAP_CHARS);
    int charCol = inputText.charIndex % MAP_CHARS;

    output.coord.x += CHAR_SIZE * charCol; 
    output.coord.y += CHAR_SIZE * charRow; 

    output.pos = float4(posX, posY, 0.0f, 1.0f);
    return output;
}
