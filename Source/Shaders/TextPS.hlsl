#include "TextOut.hlsli"
Texture2D screenTex : register(t0);
SamplerState screenSampler;

static const float3 SHADOW_COLOR = float3(0.0f, 0.0f, 0.0f);
static const float3 COLOR = float3(1.0f, 1.0f, 1.0f);
static const float THICKNESS = 0.5f;
static const float SOFTNESS = 0.075f;
static const float SHADOW_THICKNESS = 0.1f;
static const float SHADOW_BLUR = 0.4;
static const float TOTAL_THICKNESS = THICKNESS + SHADOW_THICKNESS;

float smoothstep(float a, float b, float x) {
    float t = clamp((x - a) / (b - a), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

float4 main(TextOut textOut) : SV_TARGET {
    float a = screenTex.Sample(screenSampler, textOut.coord).r; 
    float textAlpha = smoothstep(1.0f - THICKNESS - SOFTNESS, 1.0f - THICKNESS + SOFTNESS, a);
    float shadowAlpha = smoothstep(1.0f - TOTAL_THICKNESS - SHADOW_BLUR, 1.0f - TOTAL_THICKNESS + SHADOW_BLUR, a);

    float3 color = lerp(SHADOW_COLOR , COLOR, textAlpha);
    return float4(color, shadowAlpha);
}
