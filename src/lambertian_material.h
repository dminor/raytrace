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

#ifndef LAMBERTIAN_MATERIAL_H_
#define LAMBERTIAN_MATERIAL_H_

#include "material.h"
#include "ray.h"
#include "scene.h"
#include "vec.h"

struct LambertianMaterial : public Material {

    double r, g, b;
 
    LambertianMaterial() : r(1.0), g(1.0), b(1.0) {};

    virtual ~LambertianMaterial() {};

    void shade(const Scene &scene, const Vec &pt, const Vec &norm, double &r, double &g, double &b)
    {
        Light *light;

        //assume one light per scene for now 
        for (std::vector<Light *>::const_iterator itor = scene.lights.begin(); itor != scene.lights.end(); ++itor) {
            light = *itor;
        } 

        double c = norm.dot(light->direction);
        if (c < 0.0) c = 0.0;
        if (c > 1.0) c = 1.0;

        r = light->r*this->r*c;
        g = light->g*this->g*c;
        b = light->b*this->b*c; 
    }
};

#endif


