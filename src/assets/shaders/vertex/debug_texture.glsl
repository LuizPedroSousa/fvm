#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinates;

out OBJECT_COORDINATES {
    vec3 normal;
    vec2 texture_coordinates;
    vec3 fragment;
}
obj_coordinates;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main() {
    obj_coordinates.texture_coordinates = texture_coordinates;
    obj_coordinates.fragment = vec3(model * vec4(position, 1.));

    gl_Position = projection * view * vec4(obj_coordinates.fragment, 1.);
}
