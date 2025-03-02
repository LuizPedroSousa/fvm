#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinates;
layout(location = 3) in vec3 tangent;

layout(std430, binding = 0) buffer InstanceBuffer {
    mat4 models[];
};

out OBJECT_COORDINATES {
    vec2 texture;
    vec3 fragment;
    mat4 model;
    vec4 fragment_light_space;
    vec3 tangent_fragment;
    vec3 tangent_light_position;
    vec3 tangent_view_position;
    vec3 normal;
}
obj_coordinates;

uniform mat4 view;
uniform mat4 projection;
//uniform mat4 model;
uniform mat4 light_space_matrix;
uniform vec3 view_position;

struct LightExposure {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight {
    LightExposure exposure;
    vec3 direction;
    vec3 position;
};

uniform DirectionalLight directional_light;

void main() {
    mat4 model = models[gl_InstanceID];

    mat4 normalMatrix = transpose(inverse(model));

    vec3 T = normalize(normalMatrix * vec4(tangent, 0.)).xyz;
    vec3 N = normalize(normalMatrix * vec4(normal, 0.)).xyz;

    T = normalize(T - dot(T, N) * N);

    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));

    obj_coordinates.model = model;

    obj_coordinates.fragment = vec3(model * vec4(position, 1.));

    obj_coordinates.tangent_fragment = TBN * obj_coordinates.fragment;
    obj_coordinates.tangent_view_position = TBN * view_position;
    obj_coordinates.tangent_light_position = TBN * directional_light.position;

    obj_coordinates.texture = texture_coordinates;
    obj_coordinates.fragment_light_space = light_space_matrix * vec4(obj_coordinates.fragment, 1.0);

    obj_coordinates.normal = transpose(inverse(mat3(model))) * normal;

    gl_Position = projection * view * model * vec4(position, 1.0);
}
