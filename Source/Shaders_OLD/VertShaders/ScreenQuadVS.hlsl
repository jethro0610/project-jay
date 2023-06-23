#include "../FragTypes/ScreenFrag.hlsli"

ScreenFrag main(uint vId : SV_VERTEXID) {
    ScreenFrag frag;

    float2 uv = float2(vId&1, vId>>1);
    frag.uv = uv;
    frag.position = float4((uv.x-0.5f)*2.0f, -(uv.y-0.5f)*2.0f, 0.0f, 1.0f);
    return frag;
}
