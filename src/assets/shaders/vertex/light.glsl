#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinates;

out OBJECT_COORDINATES {
    vec2 texture;
    vec3 normal;
    vec3 fragment;
    mat4 model;
    vec4 fragment_light_space;
}
obj_coordinates;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 light_space_matrix;

void main() {
    obj_coordinates.model = model;
    obj_coordinates.fragment = vec3(model * vec4(position, 1.));
    obj_coordinates.normal = transpose(inverse(mat3(model))) * normal;
    obj_coordinates.texture = texture_coordinates;
    obj_coordinates.fragment_light_space = light_space_matrix * vec4(obj_coordinates.fragment, 1.0);

    gl_Position = projection * view * vec4(obj_coordinates.fragment, 1.);
}
