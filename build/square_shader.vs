#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aTextureIndex; // Texture index attribute

out vec2 TexCoord;
flat out float realTextureIndex; // Pass the texture index to the fragment shader

uniform mat4 model; // Add model matrix uniform

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); // Apply model matrix
    TexCoord = aTexCoord;
    realTextureIndex = aTextureIndex;
}