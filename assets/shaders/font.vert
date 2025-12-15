#version 330 core

layout(location = 0) in vec3 aPos;      // vertex position
layout(location = 1) in vec2 aTexCoords; // vertex UV

uniform mat4 projection;  // camera projection
uniform mat4 view;        // camera view
uniform mat4 model;       // model transform
uniform vec4 uvRect;      // glyph rectangle in atlas

out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = uvRect.xy + aTexCoords * (uvRect.zw - uvRect.xy);
}
