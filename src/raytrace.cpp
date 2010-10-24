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

#include "image.h"
#include "sphere.h"
#include "vec.h"

int main(int argc, char **argv)
{ 
    //eyepoint
    Ray r;
    r.origin.x = 0.0; r.origin.y = 0.0; r.origin.z = -10.0;

    //light source
    Vec light;
    light.x = 1.0; light.y = 0.5; light.z = -1.0;
    light.normalize();

    //scene object
    Sphere s;
    s.centre.x = 1.0; s.centre.y = 0.0; s.centre.z = 2.0;
    s.radius = 2.0;

    //intersection point and normal
    Point pt;
    Vec n;

    //create image and trace a ray for each pixel
    Image i(512, 512);
    for (size_t x = 0; x < 512; ++x) {
        for (size_t y = 0; y < 512; ++y) { 

            //calculate ray direction vector
            r.direction.x = (double)x/512.0 - 0.5;
            r.direction.y = -((double)y/512.0 - 0.5); 
            r.direction.z = 1.0;

            if (s.intersect(r, pt, n)) { 
                //Phong shading
                double c = n.dot(light);
                if (c < 0.0) c = 0.0; 
                c = pow(c, 4);
                c += 0.1; 
                if (c > 1.0) c = 1.0;

                i.set(x, y, 0.0, 0.0, c); 

            } else { 
                //dopey background
                i.set(x, y, (double)y/1024.0, (double)y/1024.0, (double)y/1024.0);
            }
        }
    }

    i.save("image.png");

    return 0;
}
