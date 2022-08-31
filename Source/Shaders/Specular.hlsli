float GetSpecular(in float3 cameraPos, in float3 fragPos, in float3 lightDir, in float3 normal) {
    float3 viewDir = normalize(cameraPos - fragPos);
    float3 reflectDir = reflect(-lightDir, normal);
    return pow(max(dot(-viewDir, reflectDir), 0.0f), 1);
}

float GetFresnel(in float3 cameraPos, in float3 fragPos, in float3 lightDir, in float3 normal) {
    float bias = 0.02f;
    float scale = 1.0f;
    float power = 4.0f;

    float3 viewDir = normalize(cameraPos - fragPos);
    return bias + scale * pow(1.0f + dot(-viewDir, normal), power);
}
