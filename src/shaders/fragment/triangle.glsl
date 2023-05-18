#version 330 core
out vec4 FragColor;

in vec3 _color;
in vec2 _texture_cord;

uniform sampler2D block_texture;

void main(){
  FragColor=texture(block_texture,_texture_cord);
}