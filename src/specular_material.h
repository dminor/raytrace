/*
Copyright (c) 2011 Daniel Minor

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

#ifndef SPECULAR_MATERIAL_H_
#define SPECULAR_MATERIAL_H_

#include "material.h"
#include "ray.h"
#include "scene.h"
#include "vec.h"

struct SpecularMaterial : public Material {

    SpecularMaterial() {};

    virtual ~SpecularMaterial() {};

    void shade(const Scene &scene, const Ray &incident, const Vec &pt,
        const Vec &norm, float &r, float &g, float &b)
    {

        //reflection
        Ray ray;
        ray.origin = pt;
        ray.direction = incident.direction - norm*incident.direction.dot(norm)*2.0;

        double tmax = std::numeric_limits<double>::max();

        Material *material;
        Vec pt2;
        Vec norm2;

        //emit ray
        if (scene.intersect(ray, 0.1, tmax, pt2, norm2, material)) {
            material->shade(scene, ray, pt2, norm2, r, g, b);
        } else {
            r = g = b = 0.0;
        }
    }
};

#endif
