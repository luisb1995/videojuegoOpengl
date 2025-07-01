#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float uCountdown;
uniform sampler2D texture1;

// Nuevos uniformes
uniform bool useSolidColor;
uniform vec3 solidColor;

void main()
{
    // ✅ Si se activa color sólido, lo aplicamos directamente y salimos
    if (useSolidColor) {
        FragColor = vec4(solidColor, 1.0);
        return;
    }

    // ⏱️ Efecto circular del countdown
    vec2 center = vec2(0.5, 0.5);
    vec2 pos = TexCoord;

    if (uCountdown >= 0.0) {
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

    // 🖼️ Textura normal
    vec4 texColor = texture(texture1, TexCoord);
    if (texColor.a < 0.1)
        discard;

    FragColor = texColor;
}
