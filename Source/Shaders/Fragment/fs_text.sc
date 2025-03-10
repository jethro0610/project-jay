$input v_texcoord0
#include <bgfx_shader.sh>

SAMPLER2D(s_sampler0, 0);

static const vec3 SHADOW_COLOR = vec3(0.0f, 0.0f, 0.0f);
static const vec3 COLOR = vec3(1.0f, 1.0f, 1.0f);
static const float THICKNESS = 0.5f;
static const float SOFTNESS = 0.75f;
static const float SHADOW_THICKNESS = 0.2f;
static const float TOTAL_THICKNESS = THICKNESS + SHADOW_THICKNESS;

float smoothstep(float a, float b, float x) {
    float t = clamp((x - a) / (b - a), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

void main() {
    float a = texture2D(s_sampler0, v_texcoord0).r; 
    float ddist = length(vec2(ddx(a), ddy(a)));

    float softness = SOFTNESS * ddist;
    float textAlpha = smoothstep(1.0f - THICKNESS - softness, 1.0f - THICKNESS + softness, a);
    float shadowAlpha = smoothstep(1.0f - TOTAL_THICKNESS - softness, 1.0f - TOTAL_THICKNESS + softness, a);

    vec3 color = lerp(SHADOW_COLOR, COLOR, textAlpha);
    gl_FragColor = vec4(color, shadowAlpha);
}
