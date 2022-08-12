#include "VertexTypes.hlsl"
#include "ConstantBuffers.hlsl"

VertOut main(VertIn inVert) {
    VertOut output;
    output.position = mul(worldViewProj, float4(inVert.position, 1.0));
    output.normal = normalize(mul(normalMat, inVert.normal));
    output.uv = float2(1.0 - inVert.uv.x, inVert.uv.y);
    return output;
}