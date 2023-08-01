SAMPLER2D(s_samplerShadow, 8);

float getShadow(vec4 sposition) {
    sposition.y = -sposition.y;
    vec3 projected = sposition.xyz / sposition.w;
    projected = projected * 0.5f + 0.5f;

    float closest = texture2D(s_samplerShadow, projected.xy).r;
    float current = projected.z;
    float shadow = current > closest ? 1.0f : 0.0f;
    
    return 1.0f - shadow;
}
