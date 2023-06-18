#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_cord;

out OBJECT_COORDINATES {
  vec2 texture;
  vec3 normal;
  vec3 fragment;
}
obj_coordinates;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() {
  obj_coordinates.fragment = vec3(model * vec4(position, 1.));
  mat3 normal_matrix       = mat3(transpose(inverse(model)));
  obj_coordinates.normal   = normalize(normal_matrix * normal);
  obj_coordinates.texture  = texture_cord;

  gl_Position = projection * view * vec4(obj_coordinates.fragment, 1.);
}