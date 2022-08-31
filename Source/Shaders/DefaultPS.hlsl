Texture2D textures[2];
SamplerState texSampler;

#include "ConstantBuffers.hlsli"
#include "VertexOutput.hlsli"
#include "Normals.hlsli"
#include "Specular.hlsli"

float4 main(VertOut outVert) : SV_TARGET {
    float4 pixelColor = textures[0].Sample(texSampler, outVert.uv); 
    float3x3 tbn = float3x3(outVert.tangent, outVert.bitangent, outVert.normal);
    float3 normal = GetNormalFromTexture(textures[1], texSampler, tbn, outVert.uv);

    float3 lightDir = float3(1.0, -1.0f, -1.0f);
    lightDir = normalize(lightDir);

    float ambient = 0.2f;
    float diffuse = max(-dot(normal, lightDir), 0.0f);
    float specular = GetSpecular(cameraPos, outVert.worldPosition.xyz, lightDir, normal);
    float fresnel = GetFresnel(cameraPos, outVert.worldPosition.xyz, lightDir, normal);
    
    /* return pixelColor; */
    return pixelColor * (diffuse + ambient + specular + fresnel);
}
