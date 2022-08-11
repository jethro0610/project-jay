#include "VertexTypes.hlsl"

Texture2D tex;
SamplerState texSampler;

float4 main(VertOut outVert) : SV_TARGET {
    //return float4(outVert.uv[0], outVert.uv[1], 0.0f, 1.0f);
    //return float4(outVert.uv[0], outVert.uv[0],outVert.uv[0], 1.0f);
    return tex.Sample(texSampler, float2(outVert.uv[0], 1.0f - outVert.uv[1]));
}