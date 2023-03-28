#include "TextOut.hlsli"
Texture2D screenTex : register(t0);
SamplerState screenSampler;

static const float3 OUTLINE_COLOR = float3(0.0f, 0.0f, 0.0f);
static const float3 COLOR = float3(1.0f, 1.0f, 1.0f);
static const float THICKNESS = 0.8f;
static const float OUTLINE_THICKNESS = 0.55f;
static const float SOFTNESS = 0.05f;

float smoothstep(float a, float b, float x) {
    float t = clamp((x - a) / (b - a), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

float4 main(TextOut textOut) : SV_TARGET {
    float a = screenTex.Sample(screenSampler, textOut.coord).r; 
    float outline = smoothstep(OUTLINE_THICKNESS - SOFTNESS, OUTLINE_THICKNESS + SOFTNESS, a);
    a = smoothstep(1.0f - THICKNESS - SOFTNESS, 1.0f - THICKNESS + SOFTNESS, a);
    float3 color = lerp(OUTLINE_COLOR, COLOR, outline);
    return float4(color, a);
}
