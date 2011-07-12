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

#include <iostream>
#include <limits>

#include "image.h" 
#include "scene.h"
#include "vec.h"
#include "view.h"

int main(int argc, char **argv)
{ 

    if (argc != 3) {
        std::cout << "usage: raytrace <view> <scene>" << std::endl;
        return 1;
    }

    //view
    View view;
    if (!view.open(argv[1])) {
        std::cout << "error: could not open view: " << argv[1] << std::endl;
        return 1; 
    }

    //scene
    Scene scene;
    if (!scene.open(argv[2])) {
        std::cout << "error: could not open scene: " << argv[2] << std::endl;
        return 1; 
    }

    //eyepoint
    Ray ray;
    ray.origin = view.pos; 

    //intersection material, point and normal
    Material *material;
    Vec pt;
    Vec n;

    Vec view_right = view.dir.cross(view.up);

    //create image and trace a ray for each pixel
    Image i(view.width, view.height);
    for (int x = 0; x < view.width; ++x) {
        for (int y = 0; y < view.height; ++y) { 
            double R = 0.0, G = 0.0, B = 0.0;

            for (int s = 0; s < view.samples; ++s) { 
                //calculate ray direction vector
                double us = view.u0 + (view.u1 - view.u0)*(x + 0.5)/view.width;
                us += (double)rand()/(double)RAND_MAX/(double)view.width;

                double vs = view.v0 + (view.v1 - view.v0)*(y + 0.5)/view.height;
                vs += (double)rand()/(double)RAND_MAX/(double)view.height;

                //negate y to correct for (0, 0) being top left rather than
                //bottom left
                ray.direction = view_right*us - view.up*vs + view.dir;
                ray.direction.normalize();

                if (scene.intersect(ray, 0.0,
                    std::numeric_limits<double>::max(), pt, n, material)) { 

                    double r, g, b;
                    material->shade(scene, ray, pt, n, r, g, b); 
                    R += r / (double)view.samples;
                    G += g / (double)view.samples;
                    B += b / (double)view.samples;
                }
            }

            i.set(x, y, R, G, B); 
        }
    }

    i.save("image.png");

    return 0;
}
