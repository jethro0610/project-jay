Texture2D textures[3];
SamplerState texSampler;

#include "../Constants/ConstantBuffers.hlsli"
#include "../FragTypes/WorldFrag.hlsli"
#include "../Functions/TriPlanar.hlsli"
#include "../Functions/ClampedBrighten.hlsli"
#include "../Functions/Specular.hlsli"

float4 main(WorldFrag frag) : SV_TARGET {
    const float TEXTURE_SIZE = 0.1f;
    const float MACRO_SIZE = 0.001f;
    const float MICRO_SIZE = 0.01f;
    
    float3 worldNormal = frag.normal;
    float3x3 tbn = float3x3(frag.tangent, frag.bitangent, frag.normal);

    float4 pixelColor = TriPlanarColor(textures[0], texSampler, frag.worldPosition.xyz, worldNormal, TEXTURE_SIZE);
    float macroStrength = TriPlanarColor(textures[2], texSampler, frag.worldPosition.xyz, worldNormal, MACRO_SIZE).r + 0.5f;
    float microStrength = TriPlanarColor(textures[2], texSampler, frag.worldPosition.xyz, worldNormal, MICRO_SIZE).r + 0.5f;
    float3 normal = TriPlanarNormal(textures[1], texSampler, tbn, frag.worldPosition.xyz, worldNormal, TEXTURE_SIZE);
    float variationStrength = lerp(0.05f, 1.0f, (microStrength * macroStrength) + 0.35f);    
    pixelColor *= variationStrength;

    float3 lightDir = float3(1.0, -1.0f, -1.0f); // TODO: Put light direction into cbuffer
    float ambient = 0.2f;
    lightDir = normalize(lightDir);
    float diffuse = max(-dot(normal, lightDir), 0.0f);
    float specular = GetSpecular(cameraPos, frag.worldPosition.xyz, lightDir, normal, 2.0f);
    specular *= 0.0f;
    float brightness = ambient + diffuse + specular;

    float fresnel = GetFresnel(cameraPos, frag.worldPosition.xyz, lightDir, normal, 1.0f, 16.0f);
    fresnel = min(fresnel, 1.0f);
    fresnel *= 0.3f;     

    // Skew brightness towards the given focus for harder shadows
    float focus = 0.6f;
    float distToFocus = abs(focus - brightness - fresnel);
    distToFocus = pow(distToFocus, 3.0f);
    distToFocus *= 512.0f;
    distToFocus = saturate(distToFocus);
    brightness = lerp(focus, brightness, distToFocus);

    float4 fresnelColor = float4(0.85f, 0.9f, 1.0f, 0.0f); 
    pixelColor = lerp(pixelColor, fresnelColor, fresnel);

    return pixelColor * brightness; 
}
