#version 330 core

layout(location=0)in vec3 position;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texture_cord;

out vec2 _texture_cord;
out vec3 _normal;
out vec3 _fragment_position;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
  _fragment_position=vec3(model*vec4(position,1.));
  _normal=mat3(transpose(inverse(model)))*normal;
  _texture_cord=texture_cord;
  
  gl_Position=projection*view*vec4(_fragment_position,1.);
}