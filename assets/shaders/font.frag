#version 330 core

in vec2 TexCoords;
uniform sampler2D spriteTexture;
uniform vec3 spriteColor;

out vec4 FragColor;

void main()
{
    vec4 sampled = texture(spriteTexture, TexCoords);
    FragColor = vec4(spriteColor, 1.0) * sampled;
}
