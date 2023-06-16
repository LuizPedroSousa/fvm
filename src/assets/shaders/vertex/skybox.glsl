#version 330 core

layout(location = 0) in vec3 position;

out vec3 _texture_cord;

uniform mat4 view;
uniform mat4 projection;

void main() {
  _texture_cord = position;
  vec4 pos      = projection * view * vec4(position, 1.0);
  gl_Position   = pos.xyww;
}