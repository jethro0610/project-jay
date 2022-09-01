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
    float specular = GetSpecular(cameraPos, outVert.worldPosition.xyz, lightDir, normal, 32.0f);
    float fresnel = GetFresnel(cameraPos, outVert.worldPosition.xyz, lightDir, normal, 0.0f, 1.0f, 2.0f);
    float brightness = diffuse + ambient + specular;
    
    // ==CEL SHADING==
    if (brightness <= 0.25f)
        brightness = 0.25f;
    else if (brightness <= 0.75f)
        brightness = 0.75f;
    else
        brightness = 0.85f;
    if (specular >= 0.5f)
        brightness = 1.5f;

    return pixelColor * (brightness + fresnel);
}
