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

#ifndef SPHERE_H_
#define SPHERE_H_

#include <cmath>

#include "intersectable.h"

struct Sphere : public Intersectable {

    Point centre;
    double radius;

    virtual bool intersect(const Ray &ray, Point &pt, Vec &norm)
    {
        Vec e_minus_c = ray.origin - centre;
        double d_dot_d = ray.direction.dot(ray.direction);
        double d_dot_e_minus_c = ray.direction.dot(e_minus_c);
        double disc = d_dot_e_minus_c*d_dot_e_minus_c - d_dot_d*(e_minus_c.dot(e_minus_c) - radius*radius);

        if (disc > 0.0) { 
            double t = -(ray.direction.dot(e_minus_c) + sqrt(disc))/d_dot_d;
            pt = ray.origin + (ray.direction*t);
            norm = (pt - centre) * (1.0/radius);
            return true;
        } else {
            return false;
        } 
    }
};

#endif

