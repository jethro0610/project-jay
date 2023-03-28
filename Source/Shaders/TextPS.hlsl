#include "TextOut.hlsli"
Texture2D screenTex : register(t0);
SamplerState screenSampler;

float4 main(TextOut textOut) : SV_TARGET {
    float3 texColor = screenTex.Sample(screenSampler, textOut.coord); 


    return float4(1.0f, 1.0f, 1.0f, texColor.r);
}
