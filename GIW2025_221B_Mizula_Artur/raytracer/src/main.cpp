
#include <iostream>
#include <cmath>
#define _USE_MATH_DEFINES
#include "rt.h"


using namespace std;
extern void draw_robot();
extern void draw_triangles();

///
int main (int argc, char * const argv[]) {

    //draw_robot();
    //draw_triangles();
    std::cout << "ray tracing ... \n";

    CScene scene;
    scene.create(); // defines sample scene parameters
    if(scene.cam.width == 0) {
        std::cout << "WARNING: scene not defined" << std::endl;
        return -1;
    }
    CRayTrace rt;
    CRay ray;
    COutput results;
    glm::mat3 ray_matrix;
    CRayTrace::compPrimaryRayMatrix(scene.cam, ray_matrix);
    std::cout << "Camera projection matrix:" << std::endl;
    PRINT_MAT3(ray_matrix);

    //ZAD5
    vector<pair<float,float>> exercise5={
        {scene.cam.width/2-1,scene.cam.height/2-1+0.5},
            {0.5,0.5},
            {scene.cam.width/2-1+0.5,0.5},
            {0.5,scene.cam.height/2-1+0.5},
            {scene.cam.width-1+0.5,scene.cam.height-1+0.5}
    };
    for (const auto& [fx,fy] : exercise5) {
        glm::vec3 dir = ray_matrix * glm::vec3(fx,fy,1);
        dir=glm::normalize(dir);
        cout<<"Dla fx,fy ("<<fx<<","<<fy<<")= ["<<round(dir.x*100)/100<<","<<round(dir.y*100)/100<<","<<round(dir.z*100)/100<<"]"<<endl;
    }
    //ZAD6
   CImage image_x(scene.cam.width, scene.cam.height);
   CImage image_y(scene.cam.width, scene.cam.height);
   CImage image_z(scene.cam.width, scene.cam.height);
   for (int j=0;j<scene.cam.height; j++) {
       for (int i=0;i<scene.cam.width; i++) {

           float fx=(float)i+0.5f;
           float fy = (float)j+0.5f;

           glm::vec3 pixelPos(fx,fy,1.0f);
           ray.pos = scene.cam.eyep;
           ray.dir = glm::normalize(ray_matrix * pixelPos);

           glm::vec3 rgb(0.0f, 0.0f, 0.0f);
           rgb.x = (ray.dir.x + 1.0f) / 2.0f;
           image_x.setPixel(i, j, rgb);

           rgb = glm::vec3(0.0f, 0.0f, 0.0f);
           rgb.y = (ray.dir.y + 1.0f) / 2.0f;
           image_y.setPixel(i, j, rgb);

           rgb = glm::vec3(0.0f, 0.0f, 1.0f);
           rgb.z = (ray.dir.z + 1.0f) / 2.0f;
           image_z.setPixel(i, j, rgb);
       }
   }
   image_x.save("C:/Users/NeuroSYS/GIW2025_221B_Mizula_Artur/images/gradient-OX.png", true);
   image_y.save("C:/Users/NeuroSYS/GIW2025_221B_Mizula_Artur/images/gradient-OY.png", true);
   image_z.save("C:/Users/NeuroSYS/GIW2025_221B_Mizula_Artur/images/gradient-OZ.png", true);

   //cv::imshow("Gradient OX", image_x.getImage());
   //cv::imshow("Gradient OY", image_y.getImage());
   //cv::imshow("Gradient OZ", image_z.getImage());
   cv::waitKey();
   std::cout << std::endl << std::endl;

   CSphere sphere(glm::vec3(0.0f, 0.0f, 0.0f), 5.0f);
   ray.pos = glm::vec3(0.0f, 0.0f, 10.0f);
   ray.dir = glm::vec3(0.3f, 0.3f, -1.0f);
   auto result = sphere.intersect(ray);
   std::cout << "t1: " << result;
    //ZAD 16 KONCOWE
    CImage image(scene.cam.width,scene.cam.height);

    for (int j = 0; j < scene.cam.height; j++) {
        for (int i = 0; i < scene.cam.width; i++) {
            float fx = float(i) + 0.5f;
            float fy = float(j) + 0.5f;

            glm::vec3 pixelPos(fx,fy,1.0f);
            ray.pos = scene.cam.eyep;
            ray.dir = glm::normalize(ray_matrix * pixelPos);

            results.col = {0, 0, 0};
            results.energy = 1.0f; // <- dodaj
            results.tree = 0;
            rt.rayTrace(scene, ray, results);

            if (results.col.x > 1 || results.col.y > 1 || results.col.z > 1) {
                results.col = {1, 1, 1};
            }

            image.setPixel(i, j, results.col);
        }
    }

    //image.save("C:/Users/NeuroSYS/GIW2025_221B_Mizula_Artur/images/final1.png", true);
    cv::imshow("Intersection", image.getImage());
    cv::waitKey(0);
    //ZAD15
    ray.pos={0,0,0};
    ray.dir={0.5f,0.5f,0.0f};
    glm::vec3 n(0, 1.0, 0);
    glm::vec3 pos = {0,0,0};
    CRay ray_reflected=rt.reflectedRay(ray,n, pos);
    std::cout << "ray_reflected.pos = ("
          << ray_reflected.pos.x << ", "
          << ray_reflected.pos.y << ", "
          << ray_reflected.pos.z << ")\n";

    std::cout << "ray_reflected.dir = ("
              << ray_reflected.dir.x << ", "
              << ray_reflected.dir.y << ", "
              << ray_reflected.dir.z << ")\n";
    return 0;
}


void draw_triangles() {

    CImage img(1000, 1000);
    
    glm::vec3 color1(0.9,0.1,0.1); // red
    glm::vec3 color2(0.1,0.9,0.1); // green
    
    // draw circle
    glm::vec3 pp(0.0,0.0,1.0);
    float radius = 0.1f;
//    img.drawCircle(pp, radius, color1);
    
    // triangle vertices
    glm::vec3 pp0(0.4, 0.3, 1);
    glm::vec3 pp1(-0.4, 0.3, 1);
    glm::vec3 pp2(0.4, -0.3, 1);
    
    // draws triangle in 2D
    img.drawLine(pp0, pp1, color1);
    img.drawLine(pp1, pp2, color1);
    img.drawLine(pp2, pp0, color1);
    
    // translation
    float tX = 0.2f; // OX translation
    float tY = 0.1f; // OY translation
    glm::mat3x3 mTrans {{1,0,0}, {0,1,0}, {tX,tY,1}}; // translation matrix
    PRINT_MAT3(mTrans);

    // translation of vertices
    pp0 = mTrans * pp0;
    pp1 = mTrans * pp1;
    pp2 = mTrans * pp2;

    // draws triangle after translation
    img.drawLine(pp0, pp1, color2);
    img.drawLine(pp1, pp2, color2);
    img.drawLine(pp2, pp0, color2);

    img.save("robot.png");
    cv::imshow("ROBOT", img.getImage());
    cv::waitKey();

}

void draw_robot() {
    CImage img(500, 500);

    glm::vec3 color3(1.0,0.5,0.0);


    // Przesunięcie o 0.5 w górę (Y + 0.5)
    glm::vec3 pp(0.0, 0.5, 1.0);
    float radius = 0.1f;
    img.drawCircle(pp, radius, color3);

    // tors vertices
    glm::vec3 pp0(-0.125, 0.375, 1);
    glm::vec3 pp1(0.125, 0.375, 1);
    glm::vec3 pp2(0.125, -0.1, 1);
    glm::vec3 pp3(-0.125, -0.1, 1);

    // Draws torso in 2D
    img.drawLine(pp0, pp1, color3);
    img.drawLine(pp1, pp2, color3);
    img.drawLine(pp2, pp3, color3);
    img.drawLine(pp3, pp0, color3);

    // hands and legs vertices
    //hand1
    glm::vec3 pp4(0.6, 0.375, 1);
    glm::vec3 pp5(0.150, 0.375, 1);
    glm::vec3 pp6(0.150, 0.25, 1);
    glm::vec3 pp7(0.6, 0.25, 1);

    img.drawLine(pp4, pp5, color3);
    img.drawLine(pp5, pp6, color3);
    img.drawLine(pp6, pp7, color3);
    img.drawLine(pp7, pp4, color3);

    float tX = -0.75f;
    //hand2
    glm::mat3x3 mTransX {{1,0,0}, {0,1,0}, {tX,0,1}};
    PRINT_MAT3(mTransX);
    glm::vec3 pp8 = mTransX * pp4;
    glm::vec3 pp9 = mTransX * pp5;
    glm::vec3 pp10 = mTransX * pp6;
    glm::vec3 pp11 = mTransX * pp7;

    img.drawLine(pp8, pp9, color3);
    img.drawLine(pp9, pp10, color3);
    img.drawLine(pp10, pp11, color3);
    img.drawLine(pp11, pp8, color3);
    //noga jeden
    float tX_1 = 0.20;
    float tY = -0.15f;
    glm::mat3x3 mTransY {{1,0,0}, {0,1,0}, {tX_1,tY,1}};
    PRINT_MAT3(mTransY);
    float angle=glm::radians(60.0f);
    glm::mat3 mRot={
        {glm::cos(angle), -glm::sin(angle), 0},
        {glm::sin(angle), glm::cos(angle), 0},
        {0, 0, 1}
    };

    pp8 = mTransY * (pp8 * mRot);
    pp9 = mTransY * (pp9 * mRot);
    pp10 = mTransY * (pp10 * mRot);
    pp11 = mTransY * (pp11 * mRot);

    img.drawLine(pp8, pp9, color3);
    img.drawLine(pp9, pp10, color3);
    img.drawLine(pp10, pp11, color3);
    img.drawLine(pp11, pp8, color3);
    //NOGA2
    float tX_2 = -0.20;
    float tY_2 = -0.15f;
    glm::mat3x3 mTransY1 {{1,0,0}, {0,1,0}, {tX_2,tY_2,1}};
    PRINT_MAT3(mTransY);
    angle=glm::radians(300.0f);
    glm::mat3 mRot1={
        {glm::cos(angle), -glm::sin(angle), 0},
        {glm::sin(angle), glm::cos(angle), 0},
        {0, 0, 1}
    };

    pp4 = mTransY1 * (pp4 * mRot1);
    pp5 = mTransY1 * (pp5 * mRot1);
    pp6 = mTransY1 * (pp6 * mRot1);
    pp7 = mTransY1 * (pp7 * mRot1);

    img.drawLine(pp4, pp5, color3);
    img.drawLine(pp5, pp6, color3);
    img.drawLine(pp6, pp7, color3);
    img.drawLine(pp7, pp4, color3);

    img.save("C:/Users/NeuroSYS/GIW2025_221B_Mizula_Artur/images/robot.png");
    cv::imshow("ROBOT", img.getImage());
    cv::waitKey();
}
