struct ParticleVert {
    float3 instancePosition : INST_POS;
    uint instanceID : SV_InstanceID;
    uint vertexID : SV_VERTEXID;
};