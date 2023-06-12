#version 330 core

in vec2 _texture_cord;

out vec4 FragColor;

uniform sampler2D _texture;

void main(){
  
  vec4 window=texture(_texture,_texture_cord);
  
  FragColor=window;
}