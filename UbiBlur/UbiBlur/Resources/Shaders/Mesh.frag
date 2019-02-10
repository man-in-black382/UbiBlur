#version 400 core

#include "CookTorrance.glsl"
#include "BlinnPhong.glsl"
#include "Lights.glsl"

// Output
layout(location = 0) out vec4 oColor;

// Input
in vec2 vTexCoords;
in vec3 vWorldPosition;
in vec3 vNormal;
in mat3 vTBN;

// Uniforms
struct Material {
    sampler2D albedoMap;
    sampler2D normalMap;
	sampler2D roughnessMap;
};

uniform Material uMaterial;
uniform vec3 uCameraPosition;
uniform int uLightingModel;

// Functions
vec3 LinearFromSRGB(vec3 sRGB) {
    return pow(sRGB, vec3(2.2));
}

vec3 SRGBFromLinear(vec3 linear) {
    return pow(linear, vec3(1.0 / 2.2));
}

vec3 FetchAlbedoMap() {
    return LinearFromSRGB(texture(uMaterial.albedoMap, vTexCoords).rgb);
}

vec3 FetchNormalMap() {
    vec3 normal = texture(uMaterial.normalMap, vTexCoords).xyz;
    return normalize(vTBN * (normal * 2.0 - 1.0));
}

float FetchRoughnessMap() {
    return texture(uMaterial.roughnessMap, vTexCoords).r;
}

void main() {
    vec3 albedo     = FetchAlbedoMap();
    vec3 N          = FetchNormalMap();
	float roughness = FetchRoughnessMap();
	float metalness = 0.0;

	DirectionalLight light;
	light.direction = normalize(vec3(-1.0, -1.0, -1.0));

	vec3 V = normalize(uCameraPosition - vWorldPosition);
	vec3 L = -light.direction;
	vec3 H = normalize(V + L);

	vec3 brdf = vec3(0.0);
	
	switch (uLightingModel) {
	case 0: 
		light.radiantFlux = vec3(1.0);
		brdf = CookTorranceBRDF(N, V, H, L, roughness, albedo, metalness, light.radiantFlux, 1.0);
		break;

	case 1:
		light.radiantFlux = vec3(0.3);
		vec3 specularReflectivity = vec3(0.3);
		float specularExponent = 16.0;
		brdf = BlinnPhongBRDF(L, N, H, albedo, specularReflectivity, specularExponent, light.radiantFlux);
		break;
	}

	brdf = SRGBFromLinear(brdf);

	oColor = vec4(brdf, 1.0);
}
