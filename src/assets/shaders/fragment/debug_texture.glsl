#version 330 core

out vec4 FragColor;

in OBJECT_COORDINATES {
    vec3 normal;
    vec2 texture_coordinates;
    vec3 fragment;
}
obj_coordinates;

void main() {
    vec3 normal = normalize(obj_coordinates.normal);

    FragColor = vec4(obj_coordinates.texture_coordinates, 0.0, 1.0);
}
