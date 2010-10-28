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

#ifndef TRIANGLE_MESH_H_
#define TRIANGLE_MESH_H_

#include "intersectable.h" 
#include "triangle.h"

#include <fstream> 
#include <vector>

struct TriangleMesh : public Intersectable {

    Sphere bounds;

    std::vector<Triangle> faces;

    bool open(const char *filename)
    {
        std::ifstream f(filename);
        if (!f) return false;

        char c;
        std::vector<Point> vertices;
        Point pt;
        Triangle t;
        size_t i, j, k;

        bounds.centre.x = bounds.centre.y = bounds.centre.z = 0.0;
        bounds.radius = 0.0;

        while (!f.eof()) {       
 
            f >> c;

            if (c == 'v') {
                f >> pt.x;
                f >> pt.y;
                f >> pt.z;

                if (fabs(pt.x) > bounds.radius) bounds.radius = fabs(pt.x);
                if (fabs(pt.y) > bounds.radius) bounds.radius = fabs(pt.y);
                if (fabs(pt.z) > bounds.radius) bounds.radius = fabs(pt.z);

                vertices.push_back(pt); 
 
            } else if (c == 'f') {
                f >> i;
                f >> j;
                f >> k;

                //build triangle
                t.a = vertices[i - 1];
                t.b = vertices[j - 1];
                t.c = vertices[k - 1];

                //calculate normal
                Vec ab = vertices[j - 1] - vertices[i - 1];
                Vec ac = vertices[k - 1] - vertices[i - 1];
                t.normal = ab.cross(ac);
                faces.push_back(t); 

            } else {
                //ignore groups, materials, etc. for now
            }

        }

        f.close(); 

        return true;

    }

    virtual bool intersect(const Ray &ray, Point &pt, Vec &norm)
    {
        if (bounds.intersect(ray, pt, norm)) { 
            for (std::vector<Triangle>::iterator itor = faces.begin(); itor != faces.end(); ++itor) {
                if (itor->intersect(ray, pt, norm)) {
                    norm.normalize();
                    return true;
                }
            }
        }

        return false;
    }

};

#endif
