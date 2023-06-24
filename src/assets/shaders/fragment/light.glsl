#version 330 core
out vec4 FragColor;

in OBJECT_COORDINATES {
  vec2 texture;
  vec3 normal;
  vec3 fragment;
}
obj_coordinates;

uniform vec3 view_position;

#define NR_POINT_LIGHTS 1
#define NR_MATERIALS 1

struct Material {
  sampler2D diffuse;
  sampler2D specular;

  float shininess;
};

struct LightAttenuation {
  float constant;
  float linear;
  float quadratic;
};

struct LightExposure {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct DirectionalLight {
  LightExposure exposure;
  vec3 direction;
};

struct SpotLight {
  LightExposure exposure;
  vec3 direction;
  vec3 position;
  float inner_cut_off;
  float outer_cut_off;
};

struct PointLight {
  vec3 position;
  LightExposure exposure;
  LightAttenuation attenuation;
};

uniform Material materials[NR_MATERIALS];
uniform SpotLight spot_light;

uniform PointLight point_lights[NR_POINT_LIGHTS];
uniform DirectionalLight directional_light;

vec3 calc_directional_light(DirectionalLight light);
vec3 calc_point_light(PointLight light, vec3 view_direction);
vec3 calc_spot_light(SpotLight light, vec3 view_direction);

float get_specular(vec3 light_direction, vec3 view_direction);
float get_diffuse(vec3 direction);
float get_attenuation(float distance, LightAttenuation attenuation);

void main() {
  vec3 view_direction = normalize(view_position - obj_coordinates.fragment);

  vec3 result = calc_point_light(point_lights[0], view_direction);

  FragColor = vec4(result, 1);
}

vec3 calc_directional_light(DirectionalLight light) {
  vec3 direction = normalize(-light.direction);

  vec3 ambient = light.exposure.ambient;

  vec3 diffuse = light.exposure.diffuse * get_diffuse(direction);

  vec3 view_direction = normalize(-obj_coordinates.fragment);
  vec3 specular =
      light.exposure.specular * get_specular(direction, view_direction);

  for (int i = 0; i < NR_MATERIALS; i++) {
    vec3 diffuse_texture =
        vec3(texture(materials[i].diffuse, obj_coordinates.texture));
    vec3 specular_texture =
        vec3(texture(materials[i].specular, obj_coordinates.texture));

    ambient *= diffuse_texture;
    diffuse *= diffuse_texture;
    diffuse *= specular_texture;
  }

  return (ambient + diffuse + specular);
};

vec3 calc_point_light(PointLight light, vec3 view_direction) {
  vec3 direction = normalize(light.position - obj_coordinates.fragment);

  vec3 ambient = light.exposure.ambient;
  vec3 diffuse = light.exposure.diffuse * get_diffuse(direction);

  vec3 specular =
      light.exposure.specular * get_specular(direction, view_direction);

  for (int i = 0; i < NR_MATERIALS; i++) {
    vec3 diffuse_texture =
        vec3(texture(materials[i].diffuse, obj_coordinates.texture));
    vec3 specular_texture =
        vec3(texture(materials[i].specular, obj_coordinates.texture));

    ambient *= diffuse_texture;
    diffuse *= diffuse_texture;
    specular *= specular_texture;
  }

  float distance = length(light.position - obj_coordinates.fragment);
  float attenuation = get_attenuation(distance, light.attenuation);

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  return ambient + diffuse + specular;
}

vec3 calc_spot_light(SpotLight light, vec3 view_direction) {
  vec3 direction = normalize(light.position - obj_coordinates.fragment);

  vec3 ambient = light.exposure.ambient;

  vec3 diffuse = light.exposure.diffuse * get_diffuse(direction);

  vec3 specular =
      light.exposure.specular * get_specular(direction, view_direction);

  float theta = dot(direction, normalize(-light.direction));
  float epsilon = light.inner_cut_off - light.outer_cut_off;
  float smooth_intensity =
      clamp((theta - light.outer_cut_off) / epsilon, 0., 1.);

  for (int i = 0; i < NR_MATERIALS; i++) {
    vec3 diffuse_texture =
        vec3(texture(materials[i].diffuse, obj_coordinates.texture));
    vec3 specular_texture =
        vec3(texture(materials[i].specular, obj_coordinates.texture));

    ambient *= diffuse_texture;
    diffuse *= diffuse_texture;
    diffuse *= specular_texture;
  }

  ambient *= smooth_intensity;
  diffuse *= smooth_intensity;
  specular *= smooth_intensity;

  return (ambient + diffuse + specular);
};

float get_specular(vec3 light_direction, vec3 view_direction) {
  vec3 halfway_direction = normalize(light_direction + view_direction);

  float result = pow(max(dot(obj_coordinates.normal, halfway_direction), 0.),
                     materials[0].shininess);

  return result;
}

float get_diffuse(vec3 direction) {
  float result = max(dot(obj_coordinates.normal, direction), 0.);
  return result;
}

float get_attenuation(float distance, LightAttenuation attenuation) {
  float result = 1. / (attenuation.constant + attenuation.linear * distance +
                       attenuation.quadratic * (distance * distance));

  return result;
}