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

#ifndef QUAT_H_
#define QUAT_H_

#include "vec.h"

struct Quat {

    double s;
    Vec v;

    Quat() : s(1.0)
    {
    }

    Quat(double angle, double x, double y, double z)
    {
        s = cos(angle/2.0);
        double scale = sin(angle/2.0);
        v.x = x*scale;
        v.y = y*scale;
        v.z = z*scale;
    }

    virtual ~Quat()
    {
    }

    Quat operator+(const Quat &other) const
    {
        Quat result;
        result.s = s + other.s;
        result.v = v + other.v;
        return result;
    }

    Quat operator-() const
    {
        Quat result;
        result.s = -s;
        result.v = -v;
        return result;
    }

    Quat operator*(double d) const
    {
        Quat result;
        result.s = s*d;
        result.v = v*d;
        return result;
    }

    Quat operator*(Quat other) const
    {
        Quat result;
        result.s = s*other.s - v.dot(other.v);
        result.v = other.v*s + v*other.s + v.cross(other.v);
        return result;
    }

    Quat operator*(Vec other) const
    {
        Quat result;
        result.s = 0.0;
        result.v = other*s + v.cross(other);
        return result;
    }

    Quat conjugate() const
    {
        Quat result;
        result.s = s;
        result.v = -v;
        return result;
    }

    void normalize()
    {
        double norm = sqrt(s*s + v.dot(v));
        if (norm > 0.0) {
            norm = 1.0/norm;
            s*=norm;
            v=v*norm;
        }
    }
};

#endif
