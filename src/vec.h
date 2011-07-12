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

#ifndef VEC_H_
#define VEC_H_

#include <cmath>

struct Vec {

    double x, y, z;

    Vec()
    {
        x = y = z = 0.0;
    }

    Vec(double x, double y, double z) : x(x), y(y), z(z)
    {
    }

    virtual ~Vec()
    {
    }

    Vec operator+(const Vec &other) const
    {
        Vec result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;
        return result;
    }

    Vec operator-(const Vec &other) const
    {
        Vec result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;
        return result;
    }


    Vec operator-() const
    {
        Vec result;
        result.x = -x;
        result.y = -y;
        result.z = -z;
        return result; 
    }

    Vec operator*(double d) const
    {
        Vec result;
        result.x = x*d;
        result.y = y*d;
        result.z = z*d;
        return result; 
    }

    Vec cross(const Vec &other) const
    {
        Vec result;
        result.x = y*other.z - z*other.y;
        result.y = -x*other.z + z*other.x;
        result.z = x*other.y - y*other.x;
        return result; 
    }

    double dot(const Vec &other) const
    {
        return x*other.x + y*other.y + z*other.z;
    }

    double magnitude() const
    {
        return sqrt(dot(*this)); 
    }

    void normalize()
    {
        double norm = sqrt(dot(*this));
        if (norm != 0.0) {
            x/=norm;
            y/=norm;
            z/=norm;
        }
    }
};

#endif
