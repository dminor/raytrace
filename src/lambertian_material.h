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

    float r, g, b;
    double reflectivity;

    LambertianMaterial() : r(1.0f), g(1.0f), b(1.0f), reflectivity(0.2) {};

    virtual ~LambertianMaterial() {};

    virtual bool isLambertian()
    {
        return true;
    }

    void shade(const Scene &scene, const Ray &incident, const Vec &pt,
        const Vec &norm, float &r, float &g, float &b) const override
    {
        Ray ray;
        ray.depth = incident.depth + 1;
        if (ray.depth_exceeded()) {
            r = scene.r;
            g = scene.g;
            b = scene.b;
            return;
        }

        ray.origin = pt;

        Vec u, v;
        norm.construct_basis(u, v);
        Vec w = Vec::sample_hemisphere_cosine_weighted();
        ray.direction = u*w.x + v*w.y + norm*w.z;
        ray.direction.normalize();

        Vec ipt;
        Vec inorm;
        float ir, ig, ib;
        Material *material;
        ir = scene.r;
        ig = scene.g;
        ib = scene.b;
        if (scene.intersect(ray, 0.001, std::numeric_limits<double>::max(),
                            ipt, inorm, material)) {
            if (material) {
                material->shade(scene, ray, ipt, inorm, ir, ig, ib);
            }
        }

        r = this->r*ir*reflectivity;
        g = this->g*ig*reflectivity;
        b = this->b*ib*reflectivity;
    }
};

#endif
