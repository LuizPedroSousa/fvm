#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void generate_line(int index);

uniform float time;

const float MAGNITUDE = 0.01;
uniform mat4 projection;

in OBJECT_COORDINATES { vec3 normal; }
obj_coordinates[];

void main() {
  generate_line(0);
  generate_line(1);
  generate_line(2);

  EndPrimitive();
}

void generate_line(int index) {
  gl_Position = projection * gl_in[index].gl_Position;
  EmitVertex();
  gl_Position =
      projection * (gl_in[index].gl_Position +
                    vec4(obj_coordinates[index].normal, 0.0) * MAGNITUDE);
  EmitVertex();
}