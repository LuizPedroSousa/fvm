#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

layout(std430, binding = 0) buffer InstanceBuffer {
    mat4 models[];
};

out OBJECT_COORDINATES {
    vec3 normal;
    mat4 model;
}
obj_coordinates;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 g_model;
uniform bool use_instacing = false;

void main() {
    mat4 model = use_instacing ? models[gl_InstanceID] : g_model;

    obj_coordinates.model = model;

    obj_coordinates.normal = transpose(inverse(mat3(model))) * normal;

    gl_Position = vec4(position, 1.);
}
