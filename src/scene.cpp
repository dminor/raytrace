
#include "scene.h"

Scene::~Scene() { 
    for (std::vector<Light *>::iterator itor = lights.begin(); itor != lights.end(); ++itor) {
        delete *itor; 
    } 
}

bool Scene::open(const char *filename)
{
    return false;
}
