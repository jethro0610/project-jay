#include "TextOut.hlsli"
#include "TextIn.hlsli"
#include "ConstantBuffers.hlsli"

static const int CHARS_PER_LINE = 32;

TextOut main(TextIn inputText) {
    TextOut output;
    int curLine = floor(inputText.instanceID / CHARS_PER_LINE);
    int curChar = inputText.instanceID % CHARS_PER_LINE;

    float inverseRatio = 1.0f / aspectRatio;
    float marginY = 0.05f;
    float marginX = marginY * inverseRatio;

    float height = 0.1f;
    float width = height * inverseRatio;

    float2 coord = float2(inputText.vertexID&1, inputText.vertexID>>1);
    float originX = coord.x * width - 1.0f;
    float originY = -coord.y * height + 1.0f;
    float posX = originX + width * curChar + marginX;
    float posY = originY - height * curLine - marginY;

    output.coord = coord;
    output.pos = float4(posX, posY, 0.0f, 1.0f);
    return output;
}
