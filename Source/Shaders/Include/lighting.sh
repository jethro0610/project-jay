#ifndef LIGHTING
#define LIGHTING_H

#ifndef NOSHADOW
#include <shadow.sh>
#endif
#include <properties.sh>

uniform vec4 u_cameraPosition;
uniform vec4 u_lightDirection;
uniform vec4 u_normalMult;

float getSpecular(vec3 wposition, vec3 normal, float power) {
    vec3 viewDir = normalize(u_cameraPosition.xyz - wposition);
    vec3 reflectDir = reflect(-u_lightDirection.xyz, normal);
    return pow(max(dot(-viewDir, reflectDir), 0.0f), power);
}

float getFresnel(vec3 wposition, vec3 normal, float power, float scale) {
    vec3 viewDir = normalize(u_cameraPosition.xyz - wposition);
    return saturate(scale * pow(1.0f + dot(-viewDir, normal), power));
}

float getDSABrightness(vec3 normal, vec3 wPosition) {
    float ambient = 0.2f;
    float diffuse = max(-dot(normal, u_lightDirection.xyz), 0.0f);
    float brightness = diffuse + ambient;

    #ifndef NOSPECULAR
    float specular = getSpecular(wPosition, normal * u_normalMult.y, PROP_SPEC_POWER);
    brightness = max(brightness, step(PROP_SPEC_THRESH, specular) * PROP_SPEC_BRIGHTNESS);
    #endif

    #ifndef NOCEL
    brightness = max(brightness, step(0.75f, brightness) * 1.0f);
    brightness = max(brightness, step(0.25f, brightness) * 0.75f);
    brightness = max(brightness, step(0.00f, brightness) * 0.25f);
    #endif

    return brightness;
}

float getBrightness(vec3 normal, vec3 wPosition, vec4 sPosition) {
    float brightness = getDSABrightness(normal, wPosition);

    #ifndef NOSHADOW
    float shadow = getShadow(sPosition);
    brightness = lerp(0.25f, brightness, shadow);
    #endif

    #ifndef NOFRESNEL
    float fresnel = getFresnel(wPosition, normal * u_normalMult.y, PROP_FRES_POWER, PROP_FRES_SCALE);
    brightness += fresnel * PROP_FRES_BRIGHTNESS;
    #endif

    return brightness;
}

#endif
