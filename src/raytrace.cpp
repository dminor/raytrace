/*
Copyright (c) 2010 Daniel Minor 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "image.h"
#include "lambertian_material.h"
#include "plane.h"
#include "scene.h"
#include "sphere.h"
#include "transform.h"
#include "triangle_mesh.h"
#include "vec.h"

const int height = 512;
const int width = 512;

int main(int argc, char **argv)
{ 
    //eyepoint
    Ray r;
    r.origin.x = 0.0; r.origin.y = 0.0; r.origin.z = -15.0;

    //scene
    Scene scene;

    //light source
    Light *light = new Light;
    light->direction.x = 1.0; light->direction.y = 0.5; light->direction.z = -1.0;
    light->direction.normalize();

    scene.lights.push_back(light);

    //teapot
    Transform *teapot = new Transform;
    teapot->translation.x = 2.0; teapot->translation.y = 2.0;
    Quat q1(3.14/4, 0, 1, 0); 
    Quat q2(-3.14/6, 0, 0, 1);
    teapot->rotation = q1*q2;

    TriangleMesh *teapot_obj = new TriangleMesh;
    teapot_obj->open("../data/teapot.obj");
    teapot->child = teapot_obj;
    LambertianMaterial *mat = new LambertianMaterial;
    mat->r = 1.0; mat->g = 0.0; mat->b = 0.0;
    teapot_obj->material = mat;

    scene.children.push_back(teapot);

    //plane
    Plane *plane = new Plane;
    plane->p.y = -1.0;
    plane->normal.x = 0.0; plane->normal.y = 1.0; plane->normal.z = 0.0;

    mat = new LambertianMaterial;
    mat->r = 0.0; mat->g = 1.0; mat->b = 0.0;
    plane->material = mat;

    scene.children.push_back(plane);

    //sphere
    Sphere *sphere = new Sphere;
    sphere->centre.x = -5.0; sphere->centre.z = 2.0;
    sphere->radius = 2.0;

    mat = new LambertianMaterial;
    mat->r = 0.0; mat->g = 0.0; mat->b = 1.0;
    sphere->material = mat;

    scene.children.push_back(sphere);

    //intersection material, point and normal
    Material *material;
    Vec pt;
    Vec n;

    //create image and trace a ray for each pixel
    Image i(width, height);
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) { 

            //calculate ray direction vector
            r.direction.x = (double)x/width - 0.5;
            r.direction.y = -((double)y/height - 0.5); 
            r.direction.z = 1.0;

            if (scene.intersect(r, pt, n, material)) { 
                double r, g, b;
                material->shade(scene, pt, n, r, g, b); 
                i.set(x, y, r, g, b); 
            } else { 
                //dopey background
                i.set(x, y, (double)y/1024.0, (double)y/1024.0, (double)y/1024.0);
            }
        }
    }

    i.save("image.png");

    return 0;
}
