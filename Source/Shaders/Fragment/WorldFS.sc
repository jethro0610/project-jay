$input v_wposition, v_normal, v_tangent, v_bitangent, v_tbn
#include <bgfx_shader.sh>
#include <Lighting.sh>
#include <WorldUniform.sh>

uniform vec4 u_cameraPosition;
uniform vec4 u_lightDirection;

SAMPLER2D(s_sampler0, 0);
SAMPLER2D(s_sampler1, 1);
SAMPLER2D(s_sampler2, 2);
SAMPLER2D(s_sampler3, 3);

static float MAX_NOISE_POS = 256.0f;
static float SAMPLE_SCALE = 1.0f / (MAX_NOISE_POS * 2.0f);

void main() {
    vec3 lightDirection = u_lightDirection.xyz; 

    vec3 color = texture2D(s_sampler1, vec2(v_wposition.x, v_wposition.z) * 0.1f).rgb;
    float macroStrength = texture2D(s_sampler3, vec2(v_wposition.x, v_wposition.z) * 0.001f).r;
    float microStrength = texture2D(s_sampler3, vec2(v_wposition.x, v_wposition.z) * 0.01f).r;
    float variationStrength = lerp(0.05f, 1.0f, (microStrength * macroStrength) + 0.35f);    
    color *= 1.5f;
    color *= variationStrength;

    vec3 normal = texture2D(s_sampler2, vec2(v_wposition.x, v_wposition.z) * 0.1f).rgb;
    normal = normal * 2.0f - 1.0f;
    normal = normalize(mul(normal, v_tbn));
    // normal = v_normal;

    float ambient = 0.2f;
    float diffuse = max(-dot(normal, lightDirection), 0.0f);
    float brightness = ambient + diffuse;

    float fresnel = getFresnel(u_cameraPosition, v_wposition, normal, 1.0f, 16.0f);
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
    
    float distToBottom = v_wposition.y - (u_minHeight - 16.0f);
    distToBottom *= 0.25f;
    distToBottom = clamp(distToBottom, 0.0f, 1.0f);
    color = lerp(vec3(0.0f, 0.0f, 0.0f), color, distToBottom);

    gl_FragColor = vec4(color * brightness, 1.0f);
}
