#include "ScreenQuadOutput.hlsli"

ScreenQuadOut main(uint vId : SV_VERTEXID) {
    ScreenQuadOut output;

    float2 coord = float2(vId&1, vId>>1);
    output.coord = coord;
    output.pos = float4((coord.x-0.5f)*2.0f, -(coord.y-0.5f)*2.0f, 0.0f, 1.0f);
    return output;
}
