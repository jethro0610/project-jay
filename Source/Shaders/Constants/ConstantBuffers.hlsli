cbuffer perFrameData : register(b0) {
    float3 cameraPos;
	float time;
    float aspectRatio;
    float4x4 viewMatrix;

    uint fpad0;
    uint fpad1;
    uint fpad2;
}

cbuffer perObjectData : register(b1){
    float4x4 worldMat;
    float4x4 worldViewProj;
    float4x4 normalMat;
};

cbuffer perSkeletonData : register(b2){
    float4x4 jointMatrices[32];
};

cbuffer perChunkData : register(b3) {
    float4 chunkPos;
    uint modifierCount;

    uint cpad0;
    uint cpad1;
    uint cpad2;
};

cbuffer perUIData : register(b4) {
    float spreadMeter;
    float3 upad0;
};
