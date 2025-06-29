#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
flat in int TextureIndex[16]; // Receive the texture indices from the vertex shader
flat in float realTextureIndex;
uniform sampler2D textures[16]; // Array of textures

void main()
{
    // Assuming you want to sample both textures based on the indices
    vec4 texColor1 = texture(textures[0], TexCoord);
    vec4 texColor2 = texture(textures[1], TexCoord);
    vec4 texColor3 = texture(textures[2], TexCoord);
    vec4 texColor4 = texture(textures[3], TexCoord);
    vec4 texColor5 = texture(textures[4], TexCoord);
    vec4 texColor6 = texture(textures[5], TexCoord);
    vec4 texColor7 = texture(textures[6], TexCoord);
    vec4 texColor8 = texture(textures[7], TexCoord);
    vec4 texColor9 = texture(textures[8], TexCoord);
    vec4 texColor10 = texture(textures[9], TexCoord);
    vec4 texColor11 = texture(textures[10], TexCoord);
    vec4 texColor12 = texture(textures[11], TexCoord);
    vec4 texColor13 = texture(textures[12], TexCoord);
    vec4 texColor14 = texture(textures[13], TexCoord);
    vec4 texColor15 = texture(textures[14], TexCoord);
    vec4 texColor16 = texture(textures[15], TexCoord);

    // Use the first texture if TextureIndex[0] == 0, otherwise use the second texture
    if (realTextureIndex == 0) {
    FragColor = mix(texColor1, texColor1, float(1));
    }

    if (realTextureIndex == 1) {
    FragColor = mix(texColor2, texColor2, float(1));
    }

        if (realTextureIndex == 2) {
    FragColor = mix(texColor3, texColor3, float(1));
    }

        if (realTextureIndex == 3) {
    FragColor = mix(texColor4, texColor4, float(1));
    }

    if (realTextureIndex == 4) {
    FragColor = mix(texColor5, texColor5, float(1));
    }

        if (realTextureIndex == 5) {
    FragColor = mix(texColor6, texColor6, float(1));
    }

    
        if (realTextureIndex == 6) {
    FragColor = mix(texColor7, texColor7, float(1));
    }

        if (realTextureIndex == 7) {
    FragColor = mix(texColor8, texColor8, float(1));
    }

    if (realTextureIndex == 8) {
    FragColor = mix(texColor9, texColor9, float(1));
    }

        if (realTextureIndex == 9) {
    FragColor = mix(texColor10, texColor10, float(1));
    }

        if (realTextureIndex == 10) {
    FragColor = mix(texColor11, texColor11, float(1));
    }

    if (realTextureIndex == 11) {
    FragColor = mix(texColor12, texColor12, float(1));
    }

        if (realTextureIndex == 12) {
    FragColor = mix(texColor13, texColor13, float(1));
    }
        if (realTextureIndex == 13) {
    FragColor = mix(texColor14, texColor14, float(1));
    }

        if (realTextureIndex == 14) {
    FragColor = mix(texColor15, texColor15, float(1));
    }

    if (realTextureIndex == 15) {
    FragColor = mix(texColor16, texColor16, float(1));
    }
}
