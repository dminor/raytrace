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

#ifndef RECTANGULAR_LIGHT_H_
#define RECTANGULAR_LIGHT_H_

#include <cstdlib>

struct RectangularLight : public Light {

    Vec pt1, pt2;
    Vec normal;

    Ray emit() override
    {
        Vec u, v;
        normal.construct_basis(u, v);
        Vec w = Vec::sample_hemisphere_cosine_weighted();
        Vec direction = u*w.x + v*w.y + normal*w.z;

        return Ray(0, random_point(), direction);
    }

    Vec random_point() override
    {
        double x = pt1.x + (pt2.x - pt1.x)*((double)(rand())/(double)RAND_MAX);
        double y = pt1.y + (pt2.y - pt1.y)*((double)(rand())/(double)RAND_MAX);
        double z = pt1.z + (pt2.z - pt1.z)*((double)(rand())/(double)RAND_MAX);

        return Vec(x, y, z);
    }

};

#endif
