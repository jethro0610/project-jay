#include "VertexTypes.hlsl"

float4 main(VertOut inVert) : SV_TARGET {
    return inVert.col;
}