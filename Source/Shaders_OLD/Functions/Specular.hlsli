float GetSpecular(in float3 cameraPos, in float3 fragPos, in float3 lightDir, in float3 normal, in float power) {
    float3 viewDir = normalize(cameraPos - fragPos);
    float3 reflectDir = reflect(-lightDir, normal);
    return pow(max(dot(-viewDir, reflectDir), 0.0f), power);
}

float GetFresnel(in float3 cameraPos, in float3 fragPos, in float3 lightDir, in float3 normal, in float scale, in float power) {
    float3 viewDir = normalize(cameraPos - fragPos);
    return saturate(scale * pow(1.0f + dot(-viewDir, normal), power));
}
