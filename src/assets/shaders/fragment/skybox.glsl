#version 330 core
out vec4 FragColor;
in vec3 _texture_cord;

uniform samplerCube _skybox;

void main() { FragColor = texture(_skybox, _texture_cord); }