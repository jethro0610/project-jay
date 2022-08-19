#include "WorldVertexInput.hlsl"
#include "VertexOutput.hlsl"
#include "ConstantBuffers.hlsl"

VertOut main(WorldVertIn inVert) {
    VertOut output;
    output.position = mul(worldViewProj, float4(inVert.position, 1.0));
    output.normal = inVert.normal;
    output.tangent = float3(0.0f, 0.0f, 0.0f);
    output.bitangent = float3(0.0f, 0.0f, 0.0f);
    output.uv = float2(0.0f, 0.0f);
    return output;
}