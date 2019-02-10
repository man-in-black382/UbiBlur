vec3 BFDiffuse(vec3 N, vec3 L, vec3 albedo, vec3 radiance) {
    float cosTheta = max(dot(N, L), 0.0);
    return albedo * radiance * cosTheta;
}

vec3 BFSpecular(vec3 N, vec3 H, float exponent, vec3 albedo, vec3 radiance) {
    float specularContribution = pow(max(dot(N, H), 0.0), exponent);
    return albedo * radiance * specularContribution;
}

vec3 BlinnPhongBRDF(vec3 L, vec3 N, vec3 H, vec3 diffuseReflectivity, vec3 specularReflectivity, float specularExponent, vec3 radiance) {
    vec3 diffuse = BFDiffuse(N, L, diffuseReflectivity, radiance);
    vec3 specular = BFSpecular(N, H, specularExponent, specularReflectivity, radiance);
    return diffuse + specular;
}
