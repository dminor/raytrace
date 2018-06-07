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

#ifndef DIELECTRIC_MATERIAL_H_
#define DIELECTRIC_MATERIAL_H_

#include <cstdlib>

#include "material.h"
#include "ray.h"
#include "scene.h"
#include "vec.h"

struct DielectricMaterial : public Material {

    double nt; //index of refraction

    DielectricMaterial() : nt(1.0) {};

    virtual ~DielectricMaterial() {};

    void shade(const Scene &scene, const Ray &incident, const Vec &pt,
        const Vec &norm, float &r, float &g, float &b)
    {

        double d_dot_n = incident.direction.dot(norm);
        double root = 1.0 - (1.0 - (d_dot_n*d_dot_n)/(nt*nt));

        //total internal reflection
        if (root < 0.0) {
            r = g = b = 1.0;
        } else {
            if ((double)rand() /(double)RAND_MAX < 0.25) {
                //reflection
                Ray ray;
                ray.depth = incident.depth + 1;
                if (ray.depth_exceeded()) {
                    return;
                }

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

                r *= 0.25;
                g *= 0.25;
                b *= 0.25;
            } else {
                //refraction
                Ray ray;
                ray.depth = incident.depth + 1;
                if (ray.depth_exceeded()) {
                    return;
                }

                ray.origin = pt;
                ray.direction = (incident.direction - norm*d_dot_n)*(1.0/nt)
                    - norm*root;

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

                r *= 0.75;
                g *= 0.75;
                b *= 0.75;
            }
        }
    }
};

#endif
