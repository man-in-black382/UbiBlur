#version 400 core

uniform sampler2D uImage;

in vec2 vTexCoords;

out vec4 oFragColor;

void main() {
    vec3 color = textureLod(uImage, vTexCoords, 0).rgb;
    oFragColor = vec4(color, 1.0);
}
