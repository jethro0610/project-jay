#include "StaticVertexInput.hlsli"
#include "VertexOutput.hlsli"
#include "ConstantBuffers.hlsli"

VertOut main(StaticVertIn inVert) {
    VertOut output;
    output.position = mul(worldViewProj, float4(inVert.position, 1.0));
    output.worldPosition = inVert.position;
    output.normal = normalize(mul(normalMat, inVert.normal));
    output.tangent = normalize(mul(normalMat, inVert.tangent));
    output.bitangent = normalize(mul(normalMat, inVert.bitangent));
    output.uv = float2(inVert.uv.x, inVert.uv.y);
    return output;
}