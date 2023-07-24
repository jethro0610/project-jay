$input v_wposition, v_normal, v_tangent, v_bitangent, v_tbn, v_texcoord0
#include <bgfx_shader.sh>
#include <Lighting.sh>
#include <Dither.sh>

uniform vec4 u_cameraPosition;
uniform vec4 u_lightDirection;

SAMPLER2D(s_sampler0, 0);

void main() {
    float alpha = texture2D(s_sampler0, v_texcoord0).r;
    vec3 lightDirection = u_lightDirection.xyz;

    DITHERDISCARD(1.0f - alpha);

    // vec3 normal = texture2D(s_sampler1, v_texcoord0).rgb;
    // normal = normal * 2.0f - 1.0f;
    // normal = normalize(mul(normal, v_tbn));
    //
    // float ambient = 0.2f;
    // float diffuse = max(-dot(normal, lightDirection), 0.0f);
    // float specular = getSpecular(u_cameraPosition.xyz, v_wposition, lightDirection, normal, 32.0f);
    // float fresnel = getFresnel(u_cameraPosition.xyz, v_wposition, normal, 1.0, 4.0f);
    // float brightness = diffuse + ambient + specular;
    //
    // if (brightness <= 0.25f)
    //     brightness = 0.25f;
    // else if (brightness <= 0.75f)
    //     brightness = 0.75f;
    // else
    //     brightness = 1.00f;
    //
    // if (specular >= 0.3f)
    //   brightness = 1.5f;

    gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
