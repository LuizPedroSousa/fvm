#version 330 core
out vec4 FragColor;

in vec2 _texture_cord;
in vec3 _normal;
in vec3 _fragment_position;
in vec3 _light_position;

uniform sampler2D block_texture;

uniform vec3 view_position;

struct Material{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Material material;
uniform Light light;

void main(){
  //ambient
  vec3 ambient=light.ambient*texture(material.diffuse,_texture_cord).rgb;
  
  //diffuse
  vec3 norm=normalize(_normal);
  vec3 light_direction=normalize(_light_position-_fragment_position);
  float diff=max(dot(norm,light_direction),0.);
  vec3 diffuse=light.diffuse*diff*texture(material.diffuse,_texture_cord).rgb;
  
  //specular
  vec3 view_direction=normalize(-_fragment_position);
  vec3 reflect_direction=reflect(-light_direction,norm);
  float spec=pow(max(dot(view_direction,reflect_direction),0.),material.shininess);
  vec3 specular=light.specular*spec*texture(material.specular,_texture_cord).rgb;
  
  vec3 result=ambient+diffuse+specular;
  
  FragColor=vec4(result,1);
}