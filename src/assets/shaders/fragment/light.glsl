#version 450 core
out vec4 FragColor;

in OBJECT_COORDINATES {
    vec2 texture;
    vec3 fragment;
    mat4 model;
    vec4 fragment_light_space;
    vec3 tangent_fragment;
    vec3 tangent_light_position;
    vec3 tangent_view_position;
    vec3 normal;
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
    vec3 position;
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
uniform int light_type;

uniform float near_plane = -10.0f;
uniform float far_plane = 20.0f;

uniform sampler2D normal_map;
uniform sampler2D displacement_map;
uniform sampler2D shadowMap;

uniform float height_scale = 0.1f;

vec3 calculate_directional_light(DirectionalLight light, vec3 normal);

vec3 calculate_directional_light_without_tangent(DirectionalLight light, vec3 normal);
vec3 calculate_directional_light_without_shadow(DirectionalLight light, vec3 normal, vec2 texture_coordinates);
// vec3 point_light(PointLight light, vec3 view_direction);
// vec3 spot_light(SpotLight light, vec3 view_direction);

float get_specular(vec3 light_direction, vec3 view_direction, vec3 normal);
float get_diffuse(vec3 light_direction, vec3 normal);
float get_attenuation(float distance, LightAttenuation attenuation);

vec3 get_diffuse_map(int i);
vec3 get_specular_map(int i);
float get_shadow(vec3 light_position, vec3 light_direction, vec3 normal);
vec2 parallax_mapping(vec2 texture_coordinates, vec3 view_direction);

void main() {
    //vec3 normal = normalize(obj_coordinates.normal);

    vec3 view_direction = normalize(obj_coordinates.tangent_view_position - obj_coordinates.tangent_fragment);
    vec2 texture_coordinates = parallax_mapping(obj_coordinates.texture, view_direction);

    // if (texture_coordinates.x > 1.0 || texture_coordinates.y > 1.0 || texture_coordinates.x < 0.0 ||
    //         texture_coordinates.y < 0.0)
    //     discard;

    vec3 normal = texture2D(normal_map, texture_coordinates).rgb * 2. - 1.;
    normal = normalize(normal);

    vec3 result;

    switch (light_type) {
        case 0:
        result = calculate_directional_light_without_shadow(directional_light, normal, texture_coordinates);
        break;
        default:
        result = vec3(0.0);
        break;
    }

    FragColor = vec4(result, 1);
}

vec3 get_diffuse_map(int i) {
    vec3 diffuse = texture(materials[i].diffuse, obj_coordinates.texture).rgb;

    if (diffuse != vec3(0)) {
        return diffuse;
    }

    return vec3(1.);
}

vec3 get_specular_map(int i) {
    vec3 specular = texture(materials[i].specular, obj_coordinates.texture).rgb;

    if (specular != vec3(0)) {
        return specular;
    }

    return vec3(0.);
}

vec3 gridSamplingDisk[20] = vec3[](
        vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
        vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
        vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
        vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
        vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
    );

float get_shadow(vec3 light_position, vec3 light_direction, vec3 normal) {
    // perspective divide
    vec3 projection_coordinates = obj_coordinates.fragment_light_space.xyz / obj_coordinates.fragment_light_space.w;
    float currentDepth = projection_coordinates.z;

    // calculate bias (based on depth map resolution and slope)
    vec3 lightDir = normalize(light_position - obj_coordinates.fragment);
    float bias = max(0.05 * (1.0 - dot(normal, light_direction)), 0.005);

    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projection_coordinates.xy + vec2(x, y) * texelSize).r;

            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.

    if (projection_coordinates.z > 1.0)

        shadow = 0.0;

    return shadow;
}

vec2 parallax_mapping(vec2 texture_coordinates, vec3 view_direction)
{
    const float min_layers = 8.0;
    const float max_layers = 32.0;
    float layers = mix(max_layers, min_layers, max(dot(vec3(0.0, 0.0, 1.0), view_direction), 0.0));

    float layer_depth = 1.0 / layers;

    float current_layer_depth = 0.0;

    vec2 p = view_direction.xy * height_scale;

    vec2 delta_texture_coordinates = p / layers;

    vec2 current_texture_coordinates = texture_coordinates;
    float current_displacement_map = texture(displacement_map, current_texture_coordinates).r;

    while (current_layer_depth < current_displacement_map)
    {
        // shift texture coordinates along direction of P
        current_texture_coordinates -= delta_texture_coordinates;

        // get depthmap value at current texture coordinates
        current_displacement_map = texture(displacement_map, current_texture_coordinates).r;
        // get depth of next layer
        current_layer_depth += layer_depth;
    }

    // get texture coordinates before collision (reverse operations)
    vec2 previous_texture_coordinates = current_texture_coordinates + delta_texture_coordinates;

    // get depth after and before collision for linear interpolation
    float after_displacement = current_displacement_map - current_layer_depth;

    float before_displacement = texture(displacement_map, previous_texture_coordinates).r - current_layer_depth
            + layer_depth;

    // interpolation of texture coordinates
    float weight = after_displacement / (after_displacement - before_displacement);

    vec2 result_texture_coordinates = previous_texture_coordinates * weight + current_texture_coordinates * (1.0 -
                    weight);

    return result_texture_coordinates;
}

vec3 calculate_directional_light_without_tangent(DirectionalLight light, vec3 normal) {
    vec3 light_direction = normalize(light.position - obj_coordinates.fragment);

    vec3 ambient = light.exposure.ambient;

    vec3 diffuse = get_diffuse(light_direction, normal) * light.exposure.diffuse;

    vec3 view_direction = normalize(view_position - obj_coordinates.fragment);

    vec3 reflect_direction = reflect(-light_direction, normal);

    vec3 specular =
        get_specular(light_direction, view_direction, normal) * light.exposure.specular;

    float shadow = get_shadow(light.position, light_direction, normal);

    vec3 color = texture(materials[0].diffuse, obj_coordinates.texture).rgb;

    return (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
};

vec3 calculate_directional_light(DirectionalLight light, vec3 normal) {
    vec3 light_direction = normalize(obj_coordinates.tangent_light_position - obj_coordinates.tangent_fragment);

    vec3 color = texture(materials[0].diffuse, obj_coordinates.texture).rgb;

    vec3 ambient = light.exposure.ambient;

    vec3 diffuse = get_diffuse(light_direction, normal) * light.exposure.diffuse;

    vec3 view_direction = normalize(obj_coordinates.tangent_view_position - obj_coordinates.tangent_fragment);

    vec3 reflect_direction = reflect(-light_direction, normal);

    vec3 specular =
        get_specular(light_direction, view_direction, normal) * light.exposure.specular;

    float shadow = get_shadow(obj_coordinates.tangent_light_position, light_direction, normal);

    return (ambient + shadow * (diffuse + specular)) * color;
};

vec3 calculate_directional_light_without_shadow(DirectionalLight light, vec3 normal, vec2 texture_coordinates) {
    vec3 light_direction = normalize(obj_coordinates.tangent_light_position - obj_coordinates.tangent_fragment);

    vec3 color = texture(materials[0].diffuse, texture_coordinates).rgb;

    vec3 ambient = light.exposure.ambient * color;

    vec3 diffuse = get_diffuse(light_direction, normal) * color;

    vec3 view_direction = normalize(obj_coordinates.tangent_view_position - obj_coordinates.tangent_fragment);

    vec3 reflect_direction = reflect(-light_direction, normal);

    vec3 specular =
        get_specular(light_direction, view_direction, normal) * light.exposure.specular;

    float shadow = get_shadow(light.position, normalize(light.position - obj_coordinates.fragment), normalize(obj_coordinates.normal));

    return (ambient + diffuse + specular);
};

// vec3 point_light(PointLight light, vec3 view_direction) {
//     vec3 direction = normalize(light.position - obj_coordinates.fragment);
//
//     vec3 ambient = light.exposure.ambient;
//     vec3 diffuse = light.exposure.diffuse * get_diffuse(direction);
//
//     vec3 specular =
//         light.exposure.specular * get_specular(direction, view_direction);
//
//     for (int i = 0; i < NR_MATERIALS; i++) {
//         vec3 diffuse_texture = get_diffuse_map(i);
//         vec3 specular_texture = get_specular_map(i);
//
//         ambient *= diffuse_texture;
//         diffuse *= diffuse_texture;
//         specular *= specular_texture;
//     }
//
//     float distance = length(light.position - obj_coordinates.fragment);
//     float attenuation = get_attenuation(distance, light.attenuation);
//
//     ambient *= attenuation;
//     diffuse *= attenuation;
//     specular *= attenuation;
//
//     return ambient + diffuse + specular;
// }
//
// vec3 spot_light(SpotLight light, vec3 view_direction) {
//     vec3 direction = normalize(light.position - obj_coordinates.fragment);
//
//     vec3 ambient = light.exposure.ambient;
//
//     vec3 diffuse = light.exposure.diffuse * get_diffuse(direction);
//
//     vec3 specular =
//         light.exposure.specular * get_specular(direction, view_direction);
//
//     float theta = dot(direction, normalize(-light.direction));
//     float epsilon = light.inner_cut_off - light.outer_cut_off;
//     float smooth_intensity =
//         clamp((theta - light.outer_cut_off) / epsilon, 0., 1.);
//
//     for (int i = 0; i < NR_MATERIALS; i++) {
//         vec3 diffuse_texture =
//             vec3(texture(materials[i].diffuse, obj_coordinates.texture));
//         vec3 specular_texture =
//             vec3(texture(materials[i].specular, obj_coordinates.texture));
//
//         ambient *= diffuse_texture;
//         diffuse *= diffuse_texture;
//         diffuse *= specular_texture;
//     }
//
//     ambient *= smooth_intensity;
//     diffuse *= smooth_intensity;
//     specular *= smooth_intensity;
//
//     return (ambient + diffuse + specular);
// };

float get_specular(vec3 light_direction, vec3 view_direction, vec3 normal) {
    vec3 halfway_direction = normalize(light_direction + view_direction);

    float halfDot = max(dot(normal, halfway_direction), 0.0);

    return pow(halfDot, 32.0);
}

float get_diffuse(vec3 light_direction, vec3 normal) {
    return max(dot(light_direction, normal), 0.);
}

float get_attenuation(float distance, LightAttenuation attenuation) {
    float result = 1. / (attenuation.constant + attenuation.linear * distance +
                attenuation.quadratic * (distance * distance));

    return result;
}
