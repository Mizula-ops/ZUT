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
    
    lightList.clear(); // clears vector with the light data
    objectList.clear(); // clears vector of pointers to objects
    CSphere* sphere1 = new CSphere({-2.5,1.3,-3},1.0);
    objectList.push_back(sphere1);
    CSphere* sphere2 = new CSphere({0,0,0},1.6);
    objectList.push_back(sphere2);
    //objectList.clear();


}

