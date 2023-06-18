#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_cord;

out OBJECT_COORDINATES { vec3 normal; }
obj_coordinates;

uniform mat4 view;
uniform mat4 model;

void main() {
  mat3 normal_matrix     = mat3(transpose(inverse(view * model)));
  obj_coordinates.normal = normalize(vec3(vec4(normal_matrix * normal, 0.)));
  gl_Position            = view * model * vec4(position, 1.);
}