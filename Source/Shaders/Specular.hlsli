float GetSpecular(in float3 cameraPos, in float3 fragPos, in float3 lightDir, in float3 normal) {
    // Ignore specular highlights where the light isn't facing the surface
    // Kinda hacky, so I probably need to figure out why the dot of the view and reflect returns positive
    // values on the opposite sided fragments
    if (-dot(lightDir, normal) <= 0.0f)
        return 0.0f;

    float3 viewDir = normalize(cameraPos - fragPos);
    float3 reflectDir = reflect(-lightDir, normal);
    return pow(max(dot(-viewDir, reflectDir), 0.0f), 8);
}
