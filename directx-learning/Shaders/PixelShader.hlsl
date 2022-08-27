Texture2D textures[2];
SamplerState texSampler;

#include "VertexOutput.hlsli"
#include "TriPlanar.hlsli"

float4 main(VertOut outVert) : SV_TARGET {
    //float4 pixelColor = textures[0].Sample(texSampler, outVert.uv * 3.0f);

    //float4 pixelColor = float4(0.5f, 0.1f, 0.25f, 1.0f);

    //float3 normal = textures[1].Sample(texSampler, outVert.uv * 3.0f);
    //normal = normalize(normal * 2.0f - 1.0f);
    //float3x3 textureMat = float3x3(outVert.tangent, outVert.bitangent, outVert.normal);
    //normal = normalize(mul(normal, textureMat));
    float3 normal = outVert.normal;
    float4 pixelColor = GetTriPlanarColor(0, outVert.worldPosition.xyz, normal, 0.05f);
	float macroStrength = GetTriPlanarColor(1, outVert.worldPosition.xyz, normal, 0.001f).r;	
	float microStrength = GetTriPlanarColor(1, outVert.worldPosition.xyz, normal, 0.005).r;	
	microStrength = (microStrength + 0.5f / 1.5f);
	float variationStrength = macroStrength * microStrength;	

	pixelColor *= (variationStrength + 0.75f) / 1.75f;
	pixelColor *= 1.5f;

    float3 lightDir = float3(1.0, -1.0f, -1.0f);
    float ambient = 0.2f;
    lightDir = normalize(lightDir);

    float diffuse = max(-dot(normal, lightDir), 0.0f);

    return pixelColor * (diffuse + ambient);
}
