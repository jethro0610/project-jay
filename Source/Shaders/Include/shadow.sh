#ifndef SHADOW_H
#define SHADOW_H

SAMPLER2D(s_samplerShadow, 14);

#ifndef BIAS_OVERRIDE
static const float BIAS = 0.0025f;
#else
static const float BIAS = BIAS_OVERRIDE;
#endif

static const float FADE_POW = 3.0f;
static const int POISSON_SAMPLES = 4;
static const vec2 POISSON_DISK[4] = {
    vec2( -0.94201624, -0.39906216 ),
    vec2( 0.94558609, -0.76890725 ),
    vec2( -0.094184101, -0.92938870 ),
    vec2( 0.34495938, 0.29387760 )
};

uniform vec4 u_shadowResolution;

float getShadow(vec4 sposition) {
    sposition.y = -sposition.y;
    vec3 projected = sposition.xyz / sposition.w;
    projected.xy = projected.xy * 0.5f + 0.5f;
    vec2 texel = 1.0f / u_shadowResolution.xy; 

    float shadow = 0.0f;
    float current = projected.z - BIAS;
    
    [unroll]
    for (int i = 0; i < POISSON_SAMPLES; i++) {
        vec2 pos = projected.xy + POISSON_DISK[i] * texel;
        float closest = texture2D(s_samplerShadow, pos).r;
        shadow += step(closest, current);
    }
    shadow /= POISSON_SAMPLES;

    float fade = distance(projected.xyz, vec3(0.5f, 0.5f, 0.5f)) * 2.0f;
    fade = clamp(pow(fade, FADE_POW), 0.0f, 1.0f);
    shadow = lerp(shadow, 0.0f, fade);

    return 1.0f - shadow;
}

float getFastShadow(vec4 sposition) {
    sposition.y = -sposition.y;
    vec3 projected = sposition.xyz / sposition.w;
    projected.xy = projected.xy * 0.5f + 0.5f;
    
    float closest = texture2D(s_samplerShadow, projected.xy).r;
    float current = projected.z - 0.0005f;
    float shadowVal = current > closest ? 1.0f : 0.0f;

    // Can't figure out how to get BGFX to use white borders, using
    // this conversion for now
    float border = step(1.0, 1.0f - closest);
    return max(border, 1.0f - shadowVal);
}

#endif
