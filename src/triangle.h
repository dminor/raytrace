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

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "intersectable.h"

struct Triangle : public Intersectable {

    Point a, b, c;
    Vec normal;

    // Intersection test for ray(p, qp) and triangle (a, b, c) with normal (n) 
    // From Ericson, C. (2005) Real-Time Collision Detection, Morgan Kauffman,
    // San Francisco, CA, pp. 190 - 192
    virtual bool intersect(const Ray &ray, Point &pt, Vec &norm)
    {
        Vec ab = b - a;
        Vec ac = c - a;

        Vec qp;
        qp.x=-ray.direction.x;
        qp.y=-ray.direction.y;
        qp.z=-ray.direction.z;

        double d = qp.dot(normal);
        if (d < 0.0) return false;

        Vec ap = ray.origin - a;
        double t = ap.dot(normal);
        if (t < 0.0) return false; 

        Vec e = qp.cross(ap);
        double v = ac.dot(e);
        if (v < 0.0 || v > d) return false; 

        double w = -ab.dot(e);
        if (w < 0.0 || v + w > d) return false; 

        double ood = 1.0/d;
        t *= ood;
        v *= ood; 
        w *= ood; 

        pt = a + ac*v + ab*w;
        norm = normal;

        return true; 
   } 
};

#endif
