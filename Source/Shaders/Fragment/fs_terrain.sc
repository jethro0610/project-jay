$input v_wposition, v_sposition, v_edgeDistance, v_normal, v_tangent, v_bitangent, v_tbn
#include <bgfx_shader.sh>
#include <lighting.sh>
#include <dither.sh>

#define SHARED_SHADER
#include <Shared_TerrainFuncs.h>

SAMPLER2D(s_color, 0);
SAMPLER2D(s_normal, 1);
SAMPLER2D(s_variation, 2);

static float MAX_NOISE_POS = 256.0f;
static float SAMPLE_SCALE = 1.0f / (MAX_NOISE_POS * 2.0f);

void main() {
    vec3 lightDirection = u_lightDirection.xyz; 

    // OPTIMIZATION: This check is run on every terrain chunk, but we
    // can just do it on a single chunk, likely by writing a different
    // shader without the fade function
    float fade = max(v_edgeDistance + 8.0f, 0.0f);
    fade *= 0.075f;
    DITHERDISCARD(fade);

    vec3 color = texture2D(s_color, vec2(v_wposition.x, v_wposition.z) * 0.1f).rgb;
    float macroStrength = texture2D(s_variation, vec2(v_wposition.x, v_wposition.z) * 0.001f).r;
    float microStrength = texture2D(s_variation, vec2(v_wposition.x, v_wposition.z) * 0.01f).r;
    float variationStrength = lerp(0.05f, 1.0f, (microStrength * macroStrength) + 0.35f);    
    color *= 1.5f;
    color *= variationStrength;

    vec3 normal = texture2D(s_normal, vec2(v_wposition.x, v_wposition.z) * 0.1f).rgb;
    normal = normal * 2.0f - 1.0f;
    normal = normalize(mul(normal, v_tbn));

    float ambient = 0.2f;
    float shadow = getShadow(v_sposition);
    float diffuse = shadow * max(-dot(normal, lightDirection), 0.0f);
    float brightness = ambient + diffuse;
    brightness = max(0.4f, brightness);

    float fresnel = getFresnel(v_wposition, normal, PROP_FRES_POWER, PROP_FRES_SCALE);
    fresnel = min(fresnel, 1.0f);
    fresnel *= 0.15f;     

    // Skew brightness towards the given focus for harder shadows
    float focus = 0.6f;
    float distToFocus = abs(focus - brightness - fresnel);
    distToFocus = pow(distToFocus, 3.0f);
    distToFocus *= 512.0f;
    distToFocus = saturate(distToFocus);
    if (brightness > focus)
        brightness = lerp(focus, brightness, distToFocus);

    float4 fresnelColor = float4(0.85f, 0.9f, 1.0f, 0.0f); 
    color = lerp(color, fresnelColor, fresnel);

    // Should also fade if height is lower than a certain threshhold
    // for holes

    gl_FragColor = vec4(color * brightness, 1.0f);
}
