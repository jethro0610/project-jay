#include "VertexTypes.hlsl"

Texture2D tex;
SamplerState texSampler;

float4 main(VertOut outVert) : SV_TARGET{
    //return float4(outVert.uv[0], outVert.uv[1], 0.0f, 1.0f);
    //return float4(outVert.uv[0], outVert.uv[0],outVert.uv[0], 1.0f);
    //float4 pixelColor = tex.Sample(texSampler, outVert.uv);

    float3 addNorm = float3(outVert.normal.x + 1.0f, outVert.normal.y + 1.0f, outVert.normal.z + 1.0f);
    return float4(addNorm / 2.0f, 1.0f);
}