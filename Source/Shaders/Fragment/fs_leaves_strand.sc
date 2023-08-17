$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <lighting.sh>
#include <properties.sh>
#include <dither.sh>

SAMPLER2D(s_sampler0, 0);

void main() {
    float alpha = texture2D(s_sampler0, v_texcoord0).a;
    DITHERDISCARD(1.0f - alpha);

    vec3 normal = v_normal;

    float ambient = 0.2f;
    float diffuse = pow(max(-dot(normal, u_lightDirection.xyz), 0.0f), 0.5f);
    float brightness = diffuse + ambient;
    brightness = max(0.6f, brightness);
    
    brightness += v_texcoord0.x * 0.2f;

    gl_FragColor = vec4(PROP_COLOR.xyz * brightness, 1.0f);
}
