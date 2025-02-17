#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out OBJECT_COORDINATES {
    vec3 normal;
}
obj_coordinates;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main() {
    obj_coordinates.normal = normal;
    gl_Position = vec4(position, 1.);
}
