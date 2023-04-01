#include "ScreenQuadOutput.hlsli"
Texture2D tex : register(t0);
SamplerState texSampler;

float4 main(ScreenQuadOut screen) : SV_TARGET {
    float val = tex.Sample(texSampler, screen.coord).r; 
    val += 1.0f;
    val /= 2.0f;
    return float4(val, val, val, 1.0f);
}
