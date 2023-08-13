$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_texcoord0
#include <bgfx_shader.sh>
#include <Lighting.sh>
#include <Dither.sh>
#include <Shadow.sh>

uniform vec4 u_cameraPosition;
uniform vec4 u_lightDirection;

SAMPLER2D(s_sampler0, 0);

void main() {
    vec4 texColor = texture2D(s_sampler0, v_texcoord0); 
    float alpha = texColor.a;
    DITHERDISCARD(1.0f - alpha);

    vec3 color = texColor.rgb;
    vec3 lightDirection = u_lightDirection.xyz;

    vec3 normal = v_normal;

    float ambient = 0.2f;
    float shadow = getShadow(v_sposition);
    float diffuse = max(-dot(normal, lightDirection), 0.0f);
    float edge = 1.0f - alpha; 
    float brightness = diffuse + ambient;

    if (brightness <= 0.25f)
        brightness = 0.25f;
    else if (brightness <= 0.75f)
        brightness = 0.75f;
    else
        brightness = 1.00f;
    brightness = lerp(brightness, 0.25f, 1.0f - shadow);

    gl_FragColor = vec4(color * (brightness + edge), 1.0f);
}
