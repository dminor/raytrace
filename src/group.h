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

#ifndef GROUP_H_
#define GROUP_H_

#include <limits>
#include <vector>

#include "intersectable.h"

struct Group : public Intersectable {

    std::vector<Intersectable *> children;

    virtual ~Group() {
        for (std::vector<Intersectable *>::iterator itor = children.begin(); itor != children.end(); ++itor) {
            delete *itor; 
        } 
    };

    virtual bool intersect(const Ray &ray, double tmin, double tmax,
        Vec &pt, Vec &norm, Material *&mat)
    {
        bool hit = false;
        double closest_distance = std::numeric_limits<double>::max(); 
        for (std::vector<Intersectable *>::iterator itor = children.begin(); itor != children.end(); ++itor) {
            Vec temp_pt;
            Vec temp_norm;
            Material *temp_mat;
            if ((*itor)->intersect(ray, tmin, tmax, temp_pt, temp_norm, temp_mat)) { 
                hit = true;
                Vec dist_vec = temp_pt - ray.origin;
                double distance = dist_vec.dot(dist_vec);
                if (distance < closest_distance) {
                    closest_distance = distance; 
                    pt = temp_pt;
                    norm = temp_norm;
                    mat = temp_mat; 
                }
            }
        } 

        return hit;
    }
};

#endif


