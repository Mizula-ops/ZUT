//
//  CRayTrace.cpp
//  rt
//
//  Created by Radoslaw Mantiuk on 22/01/2023.
//

#include "rt.h"
using namespace std;

/// \fn compPrimaryRayMatrix(CCamera cam, glm::mat3& m)
/// \brief Computation of the projection matrix.
/// \param cam Camera parameters.
/// \param m Output projection matrix.
///
////*
///
bool CRayTrace::compPrimaryRayMatrix(const CCamera& cam, glm::mat3& m) {
    float scale = cam.width / (2 * tan(glm::radians(cam.fov / 2)));
    glm::vec3 look = cam.lookp - cam.eyep;
    glm::vec3 u =glm::cross(cam.up, look);
    u/= glm::length(u);
    glm::vec3 v = glm::cross(u, look);
    v/=glm::length(v);
    glm::vec3 o = (look / glm::length(look)) * scale - (cam.width / 2.0f * u + cam.height / 2.0f * v);

    m = glm::mat3(u,v,o);

    return true;
}


bool CRayTrace::rayTrace(const CScene& scene, CRay& ray, COutput& out) {
    float tmin = numeric_limits<float>::max();
    float EPS = 0.0001f;
    bool is_intersection = false;
    CObject* hit_obj=nullptr;

    for (CObject* obj : scene.objectList) {
        float t =obj->intersect(ray);
        if (t>EPS && t<tmin) {
            tmin = t;
            hit_obj=obj;
            is_intersection = true;
        }
    }

    if (!is_intersection) {
        out.col ={0.0f,0.0f,0.0f};
        return false;
    }
    glm::vec3 p = ray.pos+tmin*ray.dir;
    glm::vec3 V = -ray.dir;

    glm::vec3 n = hit_obj->normal(p);
    glm::vec3 col = {0.0f, 0.0f, 0.0f};
    for (const CLight& light : scene.lightList) {
        col=col+light.color*hit_obj->matAmbient;

        //CIENIE
        glm::vec3 L=light.pos-p;
        float light_distance=glm::length(L);
        L=glm::normalize(L);

        CRay shadow_ray;
        shadow_ray.pos=p+EPS*L;
        shadow_ray.dir=L;
        bool in_shadow =false;
        for (CObject* obj : scene.objectList) {
            float t_shadow =obj->intersect(shadow_ray);
            if (t_shadow >EPS && t_shadow<light_distance) {
                in_shadow=true;
                break;
            }
        }
        if (in_shadow) continue;

        float cos_angle=glm::dot(n,L);
        if (cos_angle>0.001f) {
            col=col+light.color*hit_obj->matDiffuse*cos_angle;
            glm::vec3 h=glm::normalize(L+V);
            float cos_beta=glm::dot(n,h);
            if (cos_beta>0.001f) {
                col=col+light.color*hit_obj->matSpecular*pow(cos_beta,hit_obj->matShininess);
            }
        }
    }
    if (hit_obj->isTexture) {
        glm::vec2 uv = hit_obj->textureMapping(p);
        glm::vec3 tex_col = CImage::getTexel(hit_obj->texture, uv.x, uv.y);
        out.col = out.col * tex_col;
    }
    out.col = col;
    int MAX_RAY_TREE=1;
    float MIN_RAY_ENERGY=0.01f;
    if (hit_obj->reflectance > 0 && out.tree < MAX_RAY_TREE && out.energy > MIN_RAY_ENERGY) {
        COutput reflectedOut;
        reflectedOut.col = {0.0f, 0.0f, 0.0f};
        reflectedOut.energy = out.energy * hit_obj->reflectance;
        reflectedOut.tree = out.tree + 1;

        CRay reflected = reflectedRay(ray, n, p + n * EPS);
        rayTrace(scene, reflected, reflectedOut);


        out.col += reflectedOut.col * hit_obj->reflectance;
    }

    out.col = glm::clamp(out.col, 0.0f, 1.0f);
    return true;
}

CRay CRayTrace::reflectedRay(const CRay& ray, const glm::vec3& n, const glm::vec3& pos) {
    CRay reflected_ray;
    glm::vec3 v=glm::normalize(ray.dir);
    glm::vec3 r=v-(2*glm::dot(v,n))*n;
    reflected_ray.pos=pos;
    reflected_ray.dir=glm::normalize(r);
    return reflected_ray;
}

