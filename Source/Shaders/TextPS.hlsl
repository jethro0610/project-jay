#include "TextOut.hlsli"
Texture2D screenTex : register(t0);
SamplerState screenSampler;

float4 main(TextOut textOut) : SV_TARGET {
    return float4(textOut.coord.x, textOut.coord.y, 0.0f, 1.0f);
}
