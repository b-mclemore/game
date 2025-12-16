#version 330 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform vec4 uvRect;

out vec2 TexCoords;

void main()
{
    // Transform (0,0)-(1,1) to atlas sub-rectangle (u0,v0)-(u1,v1)
    TexCoords = mix(uvRect.xy, uvRect.zw, inTexCoord);
    gl_Position = projection * model * vec4(inPosition, 1.0);
}
