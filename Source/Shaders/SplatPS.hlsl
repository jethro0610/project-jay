#include "SplatFrag.hlsli"

float4 main(SplatFrag frag) : SV_TARGET {
    if (frag.pos.x == 0.0f && frag.pos.y == 0.0f && frag.pos.z == 0.0f)
        discard; 

    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
