cbuffer perFrameData : register(b0) {
	  float3 cameraPos;
	  float time;
}

cbuffer perObjectData : register(b1){
    float4x4 worldMat;
    float4x4 worldViewProj;
    float4x4 normalMat;
};

cbuffer perSkeletonData : register(b2){
    float4x4 jointMatrices[32];
};
