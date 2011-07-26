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

#ifndef PHOTON_MAP_H_ 
#define PHOTON_MAP_H_ 

#include "vec.h"

#include "kdtree.h"

struct Scene;

class PhotonMap {

    struct Photon {
        Vec location;
        double r, g, b;

        Photon()
        {
        }

        Photon(const Vec &pt) : location(pt)
        {
        }

        double &operator[](int index)
        {
            if (index == 0) return location.x;
            else if (index == 1) return location.y;
            else return location.z;
        } 

        double operator[](int index) const
        {
            if (index == 0) return location.x;
            else if (index == 1) return location.y;
            else return location.z;
        } 
    };

    Photon *photons;
    KdTree<Photon> *map;

public:

    PhotonMap();
    virtual ~PhotonMap();

    void build(const Scene &scene, int nphotons,
        bool include_direct_lighting, int max_depth);

    void query(const Vec &pt, int nphotons, double eps,
        double &r, double &g, double &b) const; 
};


#endif
