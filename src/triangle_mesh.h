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

#include <limits>

#include "intersectable.h" 
#include "sphere.h"

#include <fstream> 
#include <vector>

struct TriangleMesh : public Intersectable {

    struct Face {
        size_t i, j, k;
        Vec normal;
    };

    Sphere bounds;

    std::vector<Vec> vertices;
    std::vector<Face> faces;

    bool open(const char *filename)
    {
        std::ifstream f(filename);
        if (!f) return false;

        char c;
        Vec pt;
        Face face;
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
                f >> face.i; face.i -= 1;
                f >> face.j; face.j -= 1;
                f >> face.k; face.k -= 1;

                //calculate normal
                Vec ab = vertices[face.j] - vertices[face.i];
                Vec ac = vertices[face.k] - vertices[face.i];
                face.normal = ab.cross(ac);
                faces.push_back(face); 

            } else {
                //ignore groups, materials, etc. for now
            }

        }

        f.close(); 

        return true;

    }

    virtual bool intersect(const Ray &ray, Vec &pt, Vec &norm, Material *&mat)
    { 
        bool hit = false;
        double closest_distance = std::numeric_limits<double>::max(); 

        if (bounds.intersect(ray, pt, norm)) { 
            for (std::vector<Face>::iterator itor = faces.begin(); itor != faces.end(); ++itor) {
                Vec temp_pt;
                Vec temp_norm;
                if (intersect_face(ray, *itor, temp_pt, temp_norm)) {
                    hit = true;

                    Vec dist_vec = temp_pt - ray.origin;
                    double distance = dist_vec.dot(dist_vec);
                    if (distance < closest_distance) {
                        closest_distance = distance; 
                        pt = temp_pt;
                        norm = temp_norm;
                        mat = material;
                    }
                }
            }
        }
        if (hit) norm.normalize(); 
        return hit;
    }

    // From Ericson, C. (2005) Real-Time Collision Detection, Morgan Kauffman,
    // San Francisco, CA, pp. 190 - 192
    virtual bool intersect_face(const Ray &ray, const Face &f, Vec &pt, Vec &norm)
    {

        Vec &a = vertices[f.i];
        Vec &b = vertices[f.j];
        Vec &c = vertices[f.k]; 

        Vec ab = b - a;
        Vec ac = c - a;

        Vec qp;
        qp.x=-ray.direction.x;
        qp.y=-ray.direction.y;
        qp.z=-ray.direction.z;

        double d = qp.dot(f.normal);
        if (d < 0.0) return false;

        Vec ap = ray.origin - a;
        double t = ap.dot(f.normal);
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
        norm = f.normal;

        return true; 
   } 

};

#endif
