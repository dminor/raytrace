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

#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <limits>
#include <vector>

#include "intersectable.h"
#include "quat.h" 

struct Transform : public Intersectable {

    Vec translation;
    Quat rotation;

    Intersectable *child;

    virtual ~Transform() {};

    virtual bool intersect(const Ray &ray, double tmin, double tmax,
        Vec &pt, Vec &norm, Material *&mat)
    {
        Ray r;
        Quat conj_rotation = rotation.conjugate(); 
        r.origin = (conj_rotation*(ray.origin - translation)*rotation).v;
        r.direction = (conj_rotation*ray.direction*rotation).v; 
        return child->intersect(r, tmin, tmax, pt, norm, mat); 
    }
};

#endif


