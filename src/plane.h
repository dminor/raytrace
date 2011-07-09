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

#ifndef PLANE_H_
#define PLANE_H_

#include <cmath>

#include "intersectable.h"
#include "vec.h"

struct Plane : public Intersectable {

    Vec p;
    Vec normal;

    virtual bool intersect(const Ray &ray, double tmin, double tmax,
        Vec &pt, Vec &norm, Material *&mat)
    {
        double n = (p - ray.origin).dot(normal);
        double d = ray.direction.dot(normal);

        //check for near-zero values -- either parallel or in same plane
        if (fabs(d) < INTERSECTION_EPSILON) return false;

        //calculate hit 
        double t = n/d; 
        if (t < tmin || t > tmax) return false;

        pt = ray.origin + ray.direction*t;
        norm = normal;
        mat = material;
        return true;
    }

};

#endif
