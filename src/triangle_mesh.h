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

    std::vector<Vec> vertices;
    std::vector<Face> faces;

    virtual bool intersect(const Ray &ray, double tmin, double tmax,
        Vec &pt, Vec &norm, Material *&mat) const
    {
        bool hit = false;
        double closest_distance = std::numeric_limits<double>::max();

        for (auto& face : faces) {
            Vec temp_pt;
            Vec temp_norm;
            if (intersect_face(ray, tmin, tmax, face, temp_pt, temp_norm)) {
                hit = true;

                Vec dist_vec = temp_pt - ray.origin;
                double distance = dist_vec.dot(dist_vec);
                if (distance < closest_distance) {
                    closest_distance = distance;
                    pt = temp_pt;
                    norm = temp_norm;
                    mat = material.get();
                }
            }
        }
        if (hit) norm.normalize();
        return hit;
    }

    // From Shirley, P. et al (2009) Fundamentals of Computer Graphics, 3rd edition
    // A K Peters, Nattick, MA, pp. 77 - 80
    virtual bool intersect_face(const Ray &ray, double tmin, double tmax,
        const Face &face, Vec &pt, Vec &norm) const
    {
        const Vec &A = vertices[face.i];
        const Vec &B = vertices[face.j];
        const Vec &C = vertices[face.k];

        const Vec ab = A - B;
        const Vec ac = A - C;
        const Vec ao = A - ray.origin;

        double M = ab.x*(ac.y*ray.direction.z - ray.direction.y*ac.z) +
                   ab.y*(ray.direction.x*ac.z - ac.x*ray.direction.z) +
                   ac.z*(ac.x*ray.direction.y - ac.y*ray.direction.x);
        double t = (ac.z*(ab.x*ao.y - ao.x*ab.y) +
                    ac.y*(ao.x*ab.z - ab.x*ao.z) +
                    ac.x*(ab.y*ao.z - ao.y*ab.y))/-M;
        if (t < tmin || t > tmax) {
            return false;
        }

        double gamma = (ray.direction.z*(ab.x*ao.y - ao.x*ab.y) +
                        ray.direction.y*(ao.x*ab.z - ab.x*ao.z) +
                        ray.direction.x*(ab.y*ao.z - ao.y*ab.z))/M;
        if (gamma < 0 || gamma > 1) {
            return false;
        }

        double beta = (ao.x*(ac.y*ray.direction.z - ray.direction.y*ac.z) +
                       ao.y*(ray.direction.x*ac.z - ac.x*ray.direction.z) +
                       ao.z*(ac.x*ray.direction.y - ac.y*ray.direction.x))/M;
        if (beta < 0 || beta > (1 - gamma)) {
            return false;
        }

        pt = A + ab*-beta + ac*-gamma;
        norm = face.normal;

        return true;
   }
};

#endif
