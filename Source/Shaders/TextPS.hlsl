#include "TextOut.hlsli"
Texture2D screenTex : register(t0);
SamplerState screenSampler;

float4 main(TextOut textOut) : SV_TARGET {
    float3 color = screenTex.Sample(screenSampler, textOut.coord); 
    if (color.r == 0.0f && color.g == 0.0f && color.b == 0.0f)
        discard;

    return float4(color, 1.0f);
}
