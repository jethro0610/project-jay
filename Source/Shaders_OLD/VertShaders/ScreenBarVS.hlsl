#include "../FragTypes/ScreenFrag.hlsli"
#include "../Constants/ConstantBuffers.hlsli"

static const float HEIGHT = 0.075f;
static const float WIDTH = 0.75f;
static const float MARGIN = 0.01f;

ScreenFrag main(uint vertexId : SV_VERTEXID) {
    ScreenFrag frag;

    float2 coord = float2(vertexId&1, vertexId>>1);
    float originX = coord.x * WIDTH - 1.0f;
    float originY = -coord.y * HEIGHT + 1.0f;

    float posX = 2.0f - WIDTH - MARGIN;
    float posY = 2.0f - HEIGHT - MARGIN;

    frag.uv = coord;
    frag.position = float4(originX + posX, originY - posY, 0.0f, 1.0f);
    return frag;
}
