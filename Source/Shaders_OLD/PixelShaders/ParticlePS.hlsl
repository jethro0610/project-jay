#include "../FragTypes/ParticleFrag.hlsli"

float4 main(ParticleFrag frag) : SV_TARGET {
    float2 origin = (0.5f, 0.5f);
    if (distance(origin, frag.uv) > 0.5)
        discard;
        
    return float4(1.00f, 0.86f, 0.36f, 1.0f);
}
