#include "../VertTypes/ParticleVert.hlsli"
#include "../FragTypes/ParticleFrag.hlsli"
#include "../Constants/ConstantBuffers.hlsli"

ParticleFrag main(ParticleVert vert) {
    ParticleFrag frag;
    float2 coord = float2(vert.vertexID&1, vert.vertexID>>1);
    frag.uv = coord;
    float2 vertPos = float4((coord.x-0.5f)*2.0f, -(coord.y-0.5f)*2.0f, 0.0f, 1.0f);
    float3 vertPosWorld = vert.instancePosition + cameraRight * vertPos.x + cameraUp * vertPos.y;
    frag.position = mul(worldViewProj, float4(vertPosWorld, 1.0f)); 
    return frag;
}
