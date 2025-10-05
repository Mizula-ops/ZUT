//
//  CObject.cpp
//  rt
//
//  Created by Radoslaw Mantiuk on 22/01/2023.
//

#include "rt.h"
#include "CObject.hpp"


float CSphere::intersect(const CRay& ray) {
    glm::vec3 v = ray.pos - pos;
    float A = glm::dot(ray.dir, ray.dir);
    float B = 2.0f * glm::dot(v, ray.dir);
    float C = glm::dot(v, v) - r * r;
    float delta = B * B - 4.0f * A * C;

    if (delta < 0.0f) return {-1.0f};

    float t = (-B - sqrt(delta)) / (2.0f * A);
    //float t = (-B + sqrt(delta)) / (2.0f * A);

    return t;
}



glm::vec3 CSphere::normal(const glm::vec3& hit_pos) {
    glm::vec3 n = glm::normalize(hit_pos - pos);
    return n;
}

/// Computes texture mapping coordinates (u,v).
/// \param normal_vec Normalized normal vector at intersection point.
/// \return (u,v) texture coordinates in <0,1> range.
glm::vec2 CSphere::textureMapping(const glm::vec3& normal_vec) {
    glm::vec2 uv = {0,0};

    return uv;
}
float CTriangle::intersect(const CRay& ray) {
    float t=0.0f;
    glm::vec3 baryPosition;
    bool hit = glm::intersectRayTriangle(ray.pos, ray.dir, v0, v1, v2, baryPosition);
    if (!hit || t < 0) return -1.0f;

    return hit;
}


glm::vec3 CTriangle::normal(const glm::vec3& hit_pos) {
    glm::vec3 u = v1-v0;
    glm::vec3 v = v2-v0;
    glm::vec3 n=glm::normalize(glm::cross(u,v));
        
    return n;
}



