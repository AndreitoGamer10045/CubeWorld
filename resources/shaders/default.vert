#version 330 core

out vec4 vertexColor;
out vec2 vertexUV;

layout (location = 0) in ivec3 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec2 vUV;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(vPosition, 1.0f);
    vertexColor = vColor;
    vertexUV = vUV;
}