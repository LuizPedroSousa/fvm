#version 330 core

in vec2 _texture_cord;

out vec4 FragColor;

uniform sampler2D screen_texture;

vec3 gray_scale(vec4 texture);
vec3 invert(vec4 texture);
vec3 apply_kernel(float kernel[9], float offset);
float[9] get_blur_kernel();
float[9] get_sharpen_kernel();
float[9] get_edge_detection();

void main() {
  vec4 tex = texture(screen_texture, _texture_cord);

  FragColor = vec4(gray_scale(tex), 1.0);
}

float[9] get_blur_kernel() {

  float kernel[9] = float[](1.0 / 16, 2.0 / 16, 1.0 / 16, //
                            2.0 / 16, 4.0 / 16, 2.0 / 16, //
                            1.0 / 16, 2.0 / 16, 1.0 / 16);

  return kernel;
}

float[9] get_edge_detection() {
  float kernel[9] = float[](1, 1, 1,  //
                            1, -8, 1, //
                            1, 1, 1   //
  );

  return kernel;
}

float[9] get_sharpen_kernel() {
  float kernel[9] = float[](-1, -1, -1, //
                            -1, 9, -1,  //
                            -1, -1, -1  //
  );

  return kernel;
}

vec3 apply_kernel(float kernel[9], float offset) {
  vec2 offsets[9] = vec2[](vec2(-offset, offset), // top-left
                           vec2(0., offset),      // top-center
                           vec2(offset, offset),  // top-right

                           vec2(-offset, 0.), // center-left
                           vec2(0.),          // center-center
                           vec2(offset, 0.),  // center-right

                           vec2(-offset, -offset), // bottom-left
                           vec2(0., -offset),      // bottom-center
                           vec2(offset, -offset)   // bottom-right
  );

  vec3 sample_tex[9];

  for (int i = 0; i < 9; i++) {
    sample_tex[i] =
        vec3(texture(screen_texture, _texture_cord.st + offsets[i]));
  }

  vec3 result = vec3(0.);

  for (int i = 0; i < 9; i++) {
    result += sample_tex[i] * kernel[i];
  }

  return result;
}

vec3 invert(vec4 texture) {
  vec3 result = 1.0 - texture.rgb;

  return result;
}

vec3 gray_scale(vec4 texture) {
  float average = 0.2126 * texture.r + 0.7152 * texture.g + 0.0722 * texture.b;
  vec3 result = vec3(average, average, average);

  return result;
}