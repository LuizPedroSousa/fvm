#version 450 core
out vec4 FragColor;

in vec2 _texture_cord;

uniform sampler2D screen_texture;
uniform float gamma = 1.1;
uniform float exposure = 1.1;

void main() {
    vec3 color = texture(screen_texture, _texture_cord).rgb;

    vec3 mapped = vec3(1.) - exp(-color * exposure);

    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}
