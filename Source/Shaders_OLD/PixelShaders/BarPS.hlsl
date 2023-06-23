#include "../FragTypes/ScreenFrag.hlsli"
#include "../Constants/ConstantBuffers.hlsli"

float4 main(ScreenFrag frag) : SV_TARGET {
    float inverseRatio = 1.0f / aspectRatio;

    if (frag.uv.x < 1.0f - spreadMeter)
        return float4(0.15f, 0.15f, 0.15f, 1.0f);
    else
        return float4(1.00f, 0.86f, 0.36f, 1.0f);
}
