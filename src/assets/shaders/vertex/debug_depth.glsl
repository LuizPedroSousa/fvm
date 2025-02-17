#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinates;

out vec2 TexCoords;

void main()
{
    TexCoords = texture_coordinates;
    gl_Position = vec4(position, 1.0);
}
