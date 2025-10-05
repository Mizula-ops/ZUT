//
//  CRayTrace.cpp
//  rt
//
//  Created by Radoslaw Mantiuk on 22/01/2023.
//

#include "rt.h"
using namespace std;


bool CRayTrace::compPrimaryRayMatrix(const CCamera& cam, glm::mat3& m) {
    float scale = cam.width / (2.0f * tan((cam.fov * 3.14/180.0f)/ 2.0f));
    glm::vec3 look = cam.lookp - cam.eyep;
    glm::vec3 u =glm::cross(cam.up, look);
    u/= glm::length(u);
    glm::vec3 v = glm::cross(u, look);
    v/=glm::length(v);
    glm::vec3 o = (look / glm::length(look)) * scale - (cam.width / 2.0f * u + cam.height / 2.0f * v);

    m = glm::mat3(u,v,o);


    PRINT_MAT3(m);
    return true;

}

bool CRayTrace::rayTrace(const CScene& scene, CRay& ray, COutput& out) {

    float tmin = numeric_limits<float>::max();
    float EPS = 0.0001f;
    bool is_intersection = false;

    for (CObject* obj : scene.objectList) {
        float t =obj->intersect(ray);
        if (t>EPS && t<tmin) {
            tmin = t;
            is_intersection = true;
        }
    }
    if (is_intersection) {
        out.col ={0.0f,0.5f,0.0f};
    }else {
        out.col={0.0f,0.0f,0.0f};
    }

    return is_intersection;
}


CRay CRayTrace::reflectedRay(const CRay& ray, const glm::vec3& n, const glm::vec3& pos) {
    CRay reflected_ray;
    
    return reflected_ray;
}

