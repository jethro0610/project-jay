$input v_texcoord0
#include <bgfx_shader.sh>

SAMPLER2D(s_sampler0, 0);

vec2 getGradient(vec2 coord) {
    vec2 gradient = float2(0.0f, 0.0f);
    int sobelX[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int sobelY[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

    int index = 0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (index == 4) {
                index++;
                continue;
            }
            vec2 offset = float2(x * u_viewTexel.x, y * u_viewTexel.y);
            vec3 color = texture2D(s_sampler0, coord + offset); 
            vec3 lum = dot(color, float3(0.2126, 0.7152, 0.0722));

            gradient.x += lum * sobelX[index];
            gradient.y += lum * sobelY[index];

            index++;
        }
    }
    return gradient;
}

void main() {
    vec2 gradient = getGradient(v_texcoord0);
    float angle = 0.0f;
    if (abs(gradient.x) > 0.001f)
        angle = atan(gradient.y / gradient.x);
    float2x2 rotMat = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));

    vec2 radius = vec2(2.0f, 2.0f);
    float sampleRate = 1.0f;

    vec3 kernelMean[4] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f}
    };

    vec3 kernelVariance[4] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f}
    };
    
    vec4 kernelRanges[4] = {
        {-radius.x,     0.0f, -radius.y,     0.0f},
        {     0.0f, radius.x, -radius.y,     0.0f},
        {-radius.x,     0.0f,      0.0f, radius.y},
        {     0.0f, radius.x,      0.0f, radius.y}
    };

    for(int i = 0; i < 4; i++) {
        int totalSamples = 0;
        for (float j = kernelRanges[i].x; j <= kernelRanges[i].y; j += sampleRate) {
            for (float k = kernelRanges[i].z; k <= kernelRanges[i].w; k += sampleRate) {
                vec2 offset = vec2(j * u_viewTexel.x, k * u_viewTexel.y);
                offset = mul(offset, rotMat);
                vec3 sampleColor = texture2D(s_sampler0, v_texcoord0 + offset).rgb; 
                kernelMean[i] += sampleColor;
                kernelVariance[i] += sampleColor * sampleColor;
                totalSamples++;
            }
        }

        kernelMean[i] /= totalSamples;
        kernelVariance[i] = abs(kernelVariance[i] / totalSamples - kernelMean[i] * kernelMean[i]);
    }

    vec3 color;
    float lowestVariance = 1.0f;
    for (int i = 0; i < 4; i++) {
        float totalVariance = kernelVariance[i].r + kernelVariance[i].g + kernelVariance[i].b;

        if (totalVariance < lowestVariance) {
            lowestVariance = totalVariance;
            color = kernelMean[i];
        }
    }
    gl_FragColor = vec4(color, 1.0f);
}
