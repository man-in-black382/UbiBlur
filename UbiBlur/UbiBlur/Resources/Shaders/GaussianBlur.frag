#version 400 core

// Uniforms
uniform vec2 uRenderTargetSize;
uniform vec2 uBlurDirection;
uniform sampler2D uTexture;
uniform float uKernelWeights[64];
uniform float uTextureOffsets[64];
uniform int uKernelSize;

// Inputs
in vec2 vTexCoords;

// Outputs
out vec4 oFragColor;

// Functions
void main() {
    vec2 imageResolutionInv = 1.0 / uRenderTargetSize;

    float texelWeight = uKernelWeights[0];
    float texOffset = uTextureOffsets[0];
    vec2 currentOffset = vec2(texOffset) * imageResolutionInv * uBlurDirection;

    float mipLevel = 0.0;

    oFragColor = textureLod(uTexture, (vTexCoords + currentOffset), mipLevel) * texelWeight;

    for (int i = 1; i < uKernelSize; i++) {
        texelWeight = uKernelWeights[i];
        texOffset = uTextureOffsets[i];
        currentOffset = vec2(texOffset) * imageResolutionInv * uBlurDirection;

        oFragColor += textureLod(uTexture, (vTexCoords + currentOffset), mipLevel) * texelWeight;
        oFragColor += textureLod(uTexture, (vTexCoords - currentOffset), mipLevel) * texelWeight;
    }
}
