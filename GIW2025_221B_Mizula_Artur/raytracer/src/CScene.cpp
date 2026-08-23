//
//  CScene.cpp
//  rt
//
//  Created by Radoslaw Mantiuk on 22/01/2023.
//

#include "rt.h"

#include<fstream>

/// \fn create(void)
/// \brief Adds components to the scene.
///
void CScene::create() {

    lightList.clear();
    objectList.clear();
    cam.eyep = glm::vec3(0, -4, 10);
    cam.lookp = glm::vec3(0, 0, 0);
    cam.up = glm::vec3(0, 1, 0);
    cam.fov = 50;
    cam.width = 500;
    cam.height = 400;
    //PARAMETRY FLA final1.png
    /*
    CLight light2(glm::vec3(-3,-2,8));
    light2.color ={0.6,0.6,0.6};
    lightList.push_back(light2);
    CSphere* sphere2 = new CSphere({0,0,0},1.6);
    sphere2->matAmbient = {0.1f, 0.0f, 0.0f};
    sphere2->matDiffuse = {0.6f, 0.0f, 0.0f};
    sphere2->matSpecular = {0.7f, 0.7f, 0.7f};
    sphere2->matShininess = 30.0f;
    sphere2->reflectance = 0.0f;
    objectList.push_back(sphere2);
    CTriangle *triangle1 = new CTriangle(glm::vec3(5, 5, -5), glm::vec3(-5, 5, -5), glm::vec3(-5, -5, -5));
    triangle1->matAmbient = {0.1f, 0.1f, 0.1f};
    triangle1->matDiffuse = {0.4f, 0.4f, 0.4f};
    triangle1->matSpecular = {0.0f, 0.0f, 0.0f};
    triangle1->matShininess = 0.0f;
    triangle1->reflectance = 1.0f;
    objectList.push_back(triangle1);

    CTriangle *triangle2 = new CTriangle(glm::vec3(5, 5, -5), glm::vec3(-5, -5, -5), glm::vec3(5, -5, -5));
    triangle2->matAmbient = {0.1f, 0.1f, 0.1f};
    triangle2->matDiffuse = {0.4f, 0.4f, 0.4f};
    triangle2->matSpecular = {0.0f, 0.0f, 0.0f};
    triangle2->matShininess = 0.0f;
    triangle2->reflectance = 1.0f;
    objectList.push_back(triangle2);
    */
    //PARAMETRY DLA final.png

    CLight light1(glm::vec3(5,-2,8));
    light1.color ={0.3,0.3,0.3};
    lightList.push_back(light1);
    CLight light2(glm::vec3(-3,-2,8));
    light2.color ={0.6,0.6,0.6};
    lightList.push_back(light2);
    CSphere* sphere1 = new CSphere({-1,0,3},0.4);
    sphere1->matAmbient = {0.0f, 0.1f, 0.0f};
    sphere1->matDiffuse = {0.0f, 0.6f, 0.0f};
    sphere1->matSpecular = {0.7f, 0.7f, 0.7f};
    sphere1->matShininess = 30.0f;
    sphere1->reflectance = 0.0f;

    objectList.push_back(sphere1);

    CSphere* sphere2 = new CSphere({0,0,0},1.6);
    sphere2->matAmbient = {0.1f, 0.0f, 0.0f};
    sphere2->matDiffuse = {0.6f, 0.0f, 0.0f};
    sphere2->matSpecular = {0.7f, 0.7f, 0.7f};
    sphere2->matShininess = 30.0f;
    sphere2->reflectance = 0.0f;
    //sphere1->isTexture = true;
    //sphere1->texture = CImage::createTexture(400, 400);
    objectList.push_back(sphere2);

    CSphere* sphere3 = new CSphere({-3,-2,-2},0.6);
    sphere3->matAmbient = {0.0f, 0.0f, 0.1f};
    sphere3->matDiffuse = {0.0f, 0.0f, 0.6f};
    sphere3->matSpecular = {0.7f, 0.7f, 0.7f};
    sphere3->matShininess = 30.0f;
    sphere3->reflectance = 0.0f;

    objectList.push_back(sphere3);

    CTriangle *triangle1 = new CTriangle(glm::vec3(5, 5, -5), glm::vec3(-5, 5, -5), glm::vec3(-5, -5, -5));
    triangle1->matAmbient = {0.1f, 0.1f, 0.1f};
    triangle1->matDiffuse = {0.4f, 0.4f, 0.4f};
    triangle1->matSpecular = {0.0f, 0.0f, 0.0f};
    triangle1->matShininess = 0.0f;
    triangle1->reflectance = 1.0f;
    objectList.push_back(triangle1);

    CTriangle *triangle2 = new CTriangle(glm::vec3(5, 5, -5), glm::vec3(-5, -5, -5), glm::vec3(5, -5, -5));
    triangle2->matAmbient = {0.1f, 0.1f, 0.1f};
    triangle2->matDiffuse = {0.4f, 0.4f, 0.4f};
    triangle2->matSpecular = {0.0f, 0.0f, 0.0f};
    triangle2->matShininess = 0.0f;
    triangle2->reflectance = 1.0f;
    objectList.push_back(triangle2);

}

