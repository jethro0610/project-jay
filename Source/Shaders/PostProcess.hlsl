#include "ScreenQuadOutput.hlsli"
Texture2D screenTex : register(t0);
SamplerState screenSampler;

float2 CalcSobelGradient(float2 coord) {
    float2 gradient = float2(0.0f, 0.0f);
    float2 texelSize = float2(1.0f / 1600.0f, 1.0f / 900.0f);
    int sobelX[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int sobelY[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

    int index = 0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (index == 4) {
                index++;
                continue;
            }
            float2 offset = float2(x, y) * texelSize;
            float3 color = screenTex.Sample(screenSampler, coord + offset); 
            float3 lum = dot(color, float3(0.2126, 0.7152, 0.0722));

            gradient.x += lum * sobelX[index];
            gradient.y += lum * sobelY[index];

            index++;
        }
    }
    return gradient;
}

float4 main(ScreenQuadOut screen) : SV_TARGET {
    /* return screenTex.Sample(screenSampler, screen.coord); */
    float2 texelSize = float2(1.0f / 1600.0f, 1.0f / 900.0f);
    int2 radius = int2(2, 2);

    float2 gradient = CalcSobelGradient(screen.coord);
    float angle = 0.0f;
    if (abs(gradient.x) > 0.001f)
        angle = atan(gradient.y / gradient.x);
    float2x2 rotMat = float2x2(cos(angle), -sin(angle), sin(angle), cos(angle));

    float3 kernelMean[4] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    float3 kernelVariance[4] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    
    int4 kernelRanges[4] = {
        {-radius.x, 0.0f, -radius.y, 0.0f},
        {0.0f, radius.x, -radius.y, 0.0f},
        {-radius.x, 0.0f, 0.0f, radius.y},
        {0.0f, radius.x, 0.0f, radius.y}
    };

    for(int i = 0; i < 4; i++) {
        int totalSamples = 0;
        for (int j = kernelRanges[i].x; j <= kernelRanges[i].y; j++) {
            for (int k = kernelRanges[i].z; k <= kernelRanges[i].w; k++) {
                float2 offset = mul(float2(j, k) * texelSize, rotMat);
                /* float2 offset = float2(j, k) * texelSize; */
                float3 color = screenTex.Sample(screenSampler, screen.coord + offset); 
                kernelMean[i] += color;
                kernelVariance[i] += color * color;
                totalSamples++;
            }
        }

        kernelMean[i] /= totalSamples;
        kernelVariance[i] = abs(kernelVariance[i] / totalSamples - kernelMean[i] * kernelMean[i]);
    }

    float3 pixelColor;
    float lowestVariance = 1.0f;
    for (int i = 0; i < 4; i++) {
        float totalVariance = kernelVariance[i].r + kernelVariance[i].g + kernelVariance[i].b;

        if (totalVariance < lowestVariance) {
            lowestVariance = totalVariance;
            pixelColor = kernelMean[i];
        }
    }

    return float4(pixelColor, 1.0f); 
}
