#include "VertexTypes.hlsl"

VertOut main(VertIn inVert) {
    VertOut output;
    output.pos = float4(inVert.pos, 1.0);
    output.col = float4(inVert.col.r, inVert.col.g, inVert.col.b, 1.0);
    return output;
}