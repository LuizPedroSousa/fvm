#version 330 core
layout(points) in;
layout(line_strip, max_vertices = 2) out;

uniform float length = 1.0;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 vertex_color;

in OBJECT_COORDINATES {
    vec3 normal;
}

obj_coordinates[];

void main() {
    vec3 normal = obj_coordinates[0].normal;

    vertex_color = abs(normal);

    vec4 v0 = gl_in[0].gl_Position;

    mat4 mvp = projection * view * model;

    gl_Position = mvp * v0;
    EmitVertex();

    vec4 v1 = v0 + vec4(normal * length, 0.0);
    gl_Position = mvp * v1;
    EmitVertex();

    EndPrimitive();
}
