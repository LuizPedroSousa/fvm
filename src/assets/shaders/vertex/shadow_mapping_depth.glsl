#version 450 core

layout(location = 0) in vec3 position;

layout(std430, binding = 0) buffer InstanceBuffer {
    mat4 models[];
};

uniform mat4 light_space_matrix;
uniform mat4 g_model;

void main() {
    mat4 model = g_model;

    gl_Position = light_space_matrix * model * vec4(position, 1.0);
}
