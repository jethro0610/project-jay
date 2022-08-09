#include "VertexTypes.hlsl"
#include "ConstantBuffers.hlsl"

VertOut main(VertIn inVert) {
    VertOut output;
    output.pos = mul(float4(inVert.pos, 1.0), worldViewProj);
    output.col = float4(inVert.col.r, inVert.col.g, inVert.col.b, 1.0);
    return output;
}