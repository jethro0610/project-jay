#include "ScreenQuadOutput.hlsli"
Texture2D screenTex : register(t0);
SamplerState screenSampler;

float4 main(ScreenQuadOut screen) : SV_TARGET {
    float4 pixelColor = screenTex.Sample(screenSampler, screen.coord); 
    return pixelColor; 
}
