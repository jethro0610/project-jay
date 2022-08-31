#include "StaticVertexInput.hlsli"
#include "VertexOutput.hlsli"
#include "ConstantBuffers.hlsli"

VertOut main(StaticVertIn inVert) {
    VertOut output;
    output.position = mul(worldViewProj, float4(inVert.position, 1.0f));
    output.worldPosition = mul(worldMat, float4(inVert.position, 1.0f));
    output.normal = normalize(mul(normalMat, inVert.normal));
    output.tangent = normalize(mul(normalMat, inVert.tangent));
    output.bitangent = normalize(mul(normalMat, inVert.bitangent));
    output.uv = float2(inVert.uv.x, inVert.uv.y);
    return output;
}
