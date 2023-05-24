#version 330 core

layout(location=0)in vec3 position;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texture_cord;

out vec2 _texture_cord;
out vec3 _normal;
out vec3 _fragment_position;
out vec3 _light_position;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform vec3 light_position;

void main()
{
  gl_Position=projection*view*model*vec4(position,1.);
  _texture_cord=texture_cord;
  _fragment_position=vec3(view*model*vec4(position,1.));
  _normal=mat3(transpose(inverse(view*model)))*normal;
  _light_position=vec3(view*vec4(light_position,1.));
}

