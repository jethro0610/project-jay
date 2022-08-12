#include "VertexTypes.hlsl"
#include "ConstantBuffers.hlsl"

VertOut main(VertIn inVert) {
    VertOut output;
    output.position = mul(worldViewProj, float4(inVert.position, 1.0));
    output.normal = mul(normalMat, inVert.normal);
    output.uv = inVert.uv;
    return output;
}