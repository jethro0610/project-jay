Texture2D textures[2];
SamplerState texSampler;

#include "../Constants/ConstantBuffers.hlsli"
#include "../Functions/Normals.hlsli"
#include "../Functions/Specular.hlsli"
#include "../FragTypes/WorldFrag.hlsli"

float4 main(WorldFrag frag) : SV_TARGET {
    float4 pixelColor = textures[0].Sample(texSampler, frag.uv); 
    float3x3 tbn = float3x3(frag.tangent, frag.bitangent, frag.normal);
    float3 normal = GetNormalFromTexture(textures[1], texSampler, tbn, frag.uv);

    float3 lightDir = float3(1.0, -1.0f, -1.0f);
    lightDir = normalize(lightDir);

    float ambient = 0.2f;
    float diffuse = max(-dot(normal, lightDir), 0.0f);
    float specular = GetSpecular(cameraPos, frag.worldPosition.xyz, lightDir, normal, 32.0f);
    float fresnel = GetFresnel(cameraPos, frag.worldPosition.xyz, lightDir, normal, 1.0f, 4.0f);
    float brightness = diffuse + ambient + specular;
    
    // ==CEL SHADING==
    if (brightness <= 0.25f)
        brightness = 0.25f;
    else if (brightness <= 0.75f)
        brightness = 0.75f;
    else
        brightness = 0.85f;
    if (specular >= 0.25f)
        brightness = 1.25f;

    return pixelColor * (brightness + fresnel);
}
