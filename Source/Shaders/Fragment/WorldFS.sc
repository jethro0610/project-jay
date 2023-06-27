$input v_wposition, v_normal
#include <bgfx_shader.sh>
#include <Lighting.sh>

uniform vec4 u_cameraPosition;

SAMPLER2D(s_sampler0, 0);
SAMPLER2D(s_sampler1, 1);
SAMPLER2D(s_sampler2, 2);

static float MAX_NOISE_POS = 256.0f;
static float SAMPLE_SCALE = 1.0f / (MAX_NOISE_POS * 2.0f);

void main() {
    vec3 lightDir = normalize(vec3(1.0f, -1.0f, -1.0f));

    vec3 color = texture2D(s_sampler1, vec2(v_wposition.x, v_wposition.z) * 0.1f).rgb;
    float macroStrength = texture2D(s_sampler2, vec2(v_wposition.x, v_wposition.z) * 0.001f).r;
    float microStrength = texture2D(s_sampler2, vec2(v_wposition.x, v_wposition.z) * 0.01f).r;
    float variationStrength = lerp(0.05f, 1.0f, (microStrength * macroStrength) + 0.35f);    
    color *= 1.5f;
    color *= variationStrength;

    float ambient = 0.2f;
    float diffuse = max(-dot(v_normal, lightDir), 0.0f);
    float brightness = ambient + diffuse;

    float fresnel = getFresnel(u_cameraPosition, v_wposition, v_normal, 1.0f, 16.0f);
    fresnel = min(fresnel, 1.0f);
    fresnel *= 0.3f;     

    // Skew brightness towards the given focus for harder shadows
    float focus = 0.6f;
    float distToFocus = abs(focus - brightness - fresnel);
    distToFocus = pow(distToFocus, 3.5f);
    distToFocus *= 512.0f;
    distToFocus = saturate(distToFocus);
    brightness = lerp(focus, brightness, distToFocus);

    float4 fresnelColor = float4(0.85f, 0.9f, 1.0f, 0.0f); 
    color = lerp(color, fresnelColor, fresnel);

    gl_FragColor = vec4(color * brightness, 1.0f);
}
