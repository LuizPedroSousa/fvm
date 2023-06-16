#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_cord;

out vec2 _texture_cord;

void main() {
  _texture_cord = texture_cord;
  gl_Position   = vec4(position.x, position.y, 0, 1.);
}