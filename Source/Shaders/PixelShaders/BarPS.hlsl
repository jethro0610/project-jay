#include "../FragTypes/ScreenFrag.hlsli"
#include "../Constants/ConstantBuffers.hlsli"

static const float BORDER_HEIGHT = 0.1f;
static const float BORDER_WIDTH = 0.01f;

float4 main(ScreenFrag frag) : SV_TARGET {
    float inverseRatio = 1.0f / aspectRatio;

    if (
        frag.uv.x < BORDER_WIDTH || 
        frag.uv.x > 1.0f - BORDER_WIDTH ||
        frag.uv.y < BORDER_HEIGHT || 
        frag.uv.y > 1.0f - BORDER_HEIGHT 
    ) return float4(0.0f, 0.0f, 0.0f, 1.0f);

    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
