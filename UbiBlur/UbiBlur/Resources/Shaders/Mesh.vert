#version 400 core

// Attributes
layout (location = 0) in vec4 iPosition;
layout (location = 1) in vec3 iTexCoords;
layout (location = 2) in vec2 iLightmapCoords;
layout (location = 3) in vec3 iNormal;
layout (location = 4) in vec3 iTangent;
layout (location = 5) in vec3 iBitangent;

// Uniforms
uniform mat4 uModelMat;
uniform mat4 uNormalMat;
uniform mat4 uCameraViewMat;
uniform mat4 uCameraProjectionMat;

// Output
out vec2 vTexCoords;
out vec3 vWorldPosition;
out vec3 vNormal;
out mat3 vTBN;

// Functions
mat3 TBN() {
    vec3 T = normalize(uNormalMat * vec4(iTangent, 0.0)).xyz;
    vec3 B = normalize(uNormalMat * vec4(iBitangent, 0.0)).xyz;
    vec3 N = normalize(uNormalMat * vec4(iNormal, 0.0)).xyz;
    return mat3(T, B, N);
}

void main() {
    vec4 worldPosition = uModelMat * iPosition;
    mat3 TBN = TBN();

    vTexCoords = iTexCoords.st;
    vWorldPosition = worldPosition.xyz;
	vNormal = iNormal;
    vTBN = TBN;

    gl_Position = uCameraProjectionMat * uCameraViewMat * worldPosition;
}
