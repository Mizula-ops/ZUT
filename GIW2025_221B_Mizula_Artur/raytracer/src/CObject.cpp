//
//  CObject.cpp
//  rt
//
//  Created by Radoslaw Mantiuk on 22/01/2023.
//

#include "rt.h"
#include "CObject.hpp"


/// \fn intersect(CRay ray)
/// \brief Computes intersection between ray and sphere.
/// \param ray Ray parameters.
/// \return Distance from camera position to the closest intersection point, or negative value.
///
float CSphere::intersect(const CRay& ray) {
    glm::vec3 v = ray.pos - pos;
    float A = glm::dot(ray.dir, ray.dir);
    float B = 2.0f * glm::dot(v, ray.dir);
    float C = glm::dot(v, v) - r * r;
    float delta = B * B - 4.0f * A * C;
    if (delta < 0.0f) return {-1.0f};
    float t = (-B - sqrt(delta)) / (2.0f * A);
    return t;
}

glm::vec3 CSphere::normal(const glm::vec3& hit_pos) {
    glm::vec3 n = glm::normalize(hit_pos - pos);
    
    return n;
}

glm::vec2 CSphere::textureMapping(const glm::vec3& normal_vec) {
    glm::vec3 n = normal(normal_vec);
    float u = 0.5f + asin(n.x) /M_PI;
    float v = 0.5f - asin(n.y) / M_PI;
    glm::vec2 uv = {u, v};
    return uv;
}
//poprawa
float CTriangle::intersect(const CRay& ray) {
    float t=0.0f;
    glm::vec3 baryPosition;
    bool hit = glm::intersectRayTriangle(ray.pos, ray.dir, v0, v1, v2, baryPosition);
    if (!hit || t < 0) return -1.0f;
    glm::vec3 n=normal(ray.pos);
    float denom = glm::dot(n, ray.dir);
    t = glm::dot(v0 - ray.pos, n) / denom;
    return t;

}
glm::vec3 CTriangle::normal(const glm::vec3& hit_pos) {
    glm::vec3 u = v1-v0;
    glm::vec3 v = v2-v0;
    glm::vec3 n=glm::normalize(glm::cross(u,v));

    return n;
}



