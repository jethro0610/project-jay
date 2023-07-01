SAMPLER2D(s_sampler0, 0);
uniform vec4 u_noiseProps;

float noiseSample(vec2 position, float scale) {
    vec2 samplePos = position * u_noiseProps.y * scale;
    samplePos += vec2(0.5f, 0.5f);
    return texture2DLod(s_sampler0, samplePos, 0);
}

float noiseSampleBlob(vec2 position, float jaggedness) {
    vec2 samplePos = normalize(position) * u_noiseProps.y * jaggedness;
    samplePos += vec2(0.5f, 0.5f);
    return texture2DLod(s_sampler0, samplePos, 0);
}

