#version 330 core
layout(location=0)in vec3 position;
layout(location=1)in vec3 color;
layout(location=2)in vec2 texture_cord;

out vec3 _color;
out vec2 _texture_cord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
  gl_Position=projection*view*model*vec4(position,1.);
  _color=color;
  _texture_cord=texture_cord;
}

