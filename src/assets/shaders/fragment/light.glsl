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

vec3 calc_directional_light(DirectionalLight light, vec3 normal);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 view_direction);
vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 view_direction);
float linear_depth(float depth);

void main() {
  vec3 view_direction = normalize(view_position - obj_coordinates.fragment);

  vec3 result =
      calc_point_light(point_lights[0], obj_coordinates.normal, view_direction);

  FragColor = vec4(result, 1);
}

vec3 calc_directional_light(DirectionalLight light, vec3 normal) {
  vec3 direction = normalize(-light.direction);

  // ambient
  vec3 ambient = light.exposure.ambient;

  // diffuse
  float diff   = max(dot(normal, direction), 0.);
  vec3 diffuse = light.exposure.diffuse * diff;

  // specular
  vec3 reflect_direction = reflect(-direction, normal);
  vec3 view_direction    = normalize(-obj_coordinates.fragment);
  float spec             = pow(max(dot(view_direction, reflect_direction), 0.),
                               materials[0].shininess);

  vec3 specular = light.exposure.specular * spec;

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

vec3 calc_point_light(PointLight light, vec3 normal, vec3 view_direction) {
  vec3 direction = normalize(light.position - obj_coordinates.fragment);
  float distance = length(light.position - obj_coordinates.fragment);
  float attenuation =
      1. / (light.attenuation.constant + light.attenuation.linear * distance +
            light.attenuation.quadratic * (distance * distance));

  // ambient
  vec3 ambient = light.exposure.ambient;

  // diffuse
  float diff   = max(dot(normal, direction), 0.);
  vec3 diffuse = light.exposure.diffuse * diff;

  // specular
  vec3 reflect_direction = reflect(-direction, normal);

  vec3 dir = normalize(light.position - obj_coordinates.fragment);

  float spec =
      pow(max(dot(dir, reflect_direction), 0.), materials[0].shininess);
  vec3 specular = light.exposure.specular * spec;

  for (int i = 0; i < NR_MATERIALS; i++) {
    vec3 diffuse_texture =
        vec3(texture(materials[i].diffuse, obj_coordinates.texture));
    vec3 specular_texture =
        vec3(texture(materials[i].specular, obj_coordinates.texture));

    ambient *= diffuse_texture;
    diffuse *= diffuse_texture;
    specular *= specular_texture;
  }

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  return ambient + diffuse + specular;
}

vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 view_direction) {
  vec3 direction = normalize(light.position - obj_coordinates.fragment);

  // ambient
  vec3 ambient = light.exposure.ambient;

  // diffuse
  float diff   = max(dot(normal, direction), 0.);
  vec3 diffuse = light.exposure.diffuse * diff;

  // specular

  vec3 reflect_direction = reflect(-direction, normal);
  float spec = pow(max(dot(view_direction, reflect_direction), 0.), 32.);

  vec3 specular = light.exposure.specular * spec;

  float theta   = dot(direction, normalize(-light.direction));
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
