#include "VertexTypes.hlsl"
#include "ConstantBuffers.hlsl"

VertOut main(VertIn inVert) {
    VertOut output;
    output.pos = mul(float4(inVert.pos, 1.0), worldViewProj);
    return output;
}