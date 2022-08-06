#include "VertexTypes.hlsl"

vertexOut main(vertexIn input) {
    vertexOut output = (vertexOut)0;
    output.positionClip = float4(input.positionLocal, 1.0);
    return output;
}