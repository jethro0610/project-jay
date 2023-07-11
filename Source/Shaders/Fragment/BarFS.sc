$input v_texcoord0
#include <bgfx_shader.sh>

uniform vec4 u_meter;

void main() {
    float meterValue = u_meter.x / u_meter.y;
    vec4 color = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (v_texcoord0.x < 1.0f - meterValue)
        color = vec4(0.15f, 0.15f, 0.15f, 1.0f);
    else
        color = vec4(1.00f, 0.86f, 0.36f, 1.0f);

    gl_FragColor = color;
}
