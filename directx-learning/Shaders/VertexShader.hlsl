#include "VertexTypes.hlsl"
#include "ConstantBuffers.hlsl"

VertOut main(VertIn inVert) {
    VertOut output;
    output.pos = mul(worldViewProj, float4(inVert.pos, 1.0));
    output.uv = inVert.uv;
    return output;
}