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
#include <cstdlib>

const int BASIS_EPS = 0.001;
const double pi = 3.14159265358979;

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

    static Vec sample_hemisphere()
    {
        Vec result;

        double u1 = (double)rand()/(double)RAND_MAX;
        double u2 = (double)rand()/(double)RAND_MAX;

        result.x = cos(2*pi*u2)*2*sqrt(1.0 - u1*u1);
        result.y = sin(2*pi*u2)*2*sqrt(1.0 - u1*u1);
        result.z = u1;

        return result;
    }

    static Vec sample_sphere()
    {
        Vec result;

        double u1 = (double)rand()/(double)RAND_MAX;
        double u2 = (double)rand()/(double)RAND_MAX;

        result.x = cos(2*pi*u2)*2*sqrt(u1*(1.0 - u1));
        result.y = sin(2*pi*u2)*2*sqrt(u1*(1.0 - u1));
        result.z = 1.0 - 2.0*u1;

        return result;
    }

    static Vec sample_hemisphere_cosine_weighted()
    {
        Vec result;
        double u1 = (double)rand()/(double)RAND_MAX;
        double u2 = (double)rand()/(double)RAND_MAX;

        double th = 2*pi*u2;
        double r = sqrt(u1);
        result.x = cos(th)*r;
        result.y = sin(th)*r;
        result.z = 1.0 - result.x*result.x - result.y*result.y;
        if (result.z <= 0.0) result.z = 0.0;
        else result.z = sqrt(result.z);

        /*
        double r1 = (double)rand()/(double)RAND_MAX;
        double r2 = (double)rand()/(double)RAND_MAX;
        double cos_theta = sqrt(1.0 - r1);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
        double phi = 2.0*pi*r2;

        result.x = cos(phi)*sin_theta;
        result.y = sin(phi)*sin_theta;
        result.z = cos_theta;
        */

        return result;
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

    Vec operator*(const Vec &other) const
    {
        Vec result;
        result.x = x*other.x;
        result.y = y*other.y;
        result.z = z*other.z;
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

    //construct non-unique orthonormal basis from this vector
    void construct_basis(Vec &u, Vec &v) const
    {
        Vec n(1.0, 0.0, 0.0);
        Vec m(0.0, 1.0, 0.0);
        u = cross(n);
        if (u.magnitude() < BASIS_EPS) {
            u = cross(m);
        }

        v = cross(u);
    }

    void print() const
    {
        printf("%f %f %f", x, y, z);
    }
};

#endif
