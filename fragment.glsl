#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
// Nuevos uniformes
uniform bool useSolidColor;
uniform vec3 solidColor;

uniform float uCountdown;
uniform sampler2D texture1;


void main()
{
    // ✅ Si se activa color sólido, lo aplicamos inmediatamente y salimos
    if (useSolidColor) {
        FragColor = vec4(solidColor, 1.0);
        return;
    }

    // ⏱️ Efecto circular de countdown (solo si no usamos color sólido)
    if (uCountdown >= 0.0) {
        vec2 center = vec2(0.5, 0.5);
        vec2 pos = TexCoord;

        float dist = distance(pos, center);
        if (dist > 0.5)
            discard;

        vec2 dir = normalize(pos - center);
        float angle = atan(dir.y, dir.x);
        if (angle < 0.0) angle += 2.0 * 3.14159;

        float cutoff = mix(0.0, 2.0 * 3.14159, uCountdown / 5.0);
        if (angle > cutoff)
            discard;
    }

    // 🖼️ Caso normal: textura con transparencia
    vec4 texColor = texture(texture1, TexCoord);
    if (texColor.a < 0.1)
        discard;

    FragColor = texColor;
}
