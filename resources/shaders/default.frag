#version 330 core

out vec4 fragColor;

in vec4 vertexColor;
in vec2 vertexUV;

uniform sampler2D texture1;

void main()
{
    vec4 textureColor = texture(texture1, vertexUV);
    fragColor = textureColor * vertexColor;
}