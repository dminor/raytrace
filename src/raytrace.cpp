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

#include "image.h" 
#include "scene.h"
#include "vec.h"

const int height = 128;//512;
const int width = 128;//512;

int main(int argc, char **argv)
{ 

    if (argc != 2) {
        std::cout << "usage: raytrace <filename>" << std::endl;
        return 1;
    }

    //eyepoint
    Ray r;
    r.origin.x = 0.0; r.origin.y = 0.0; r.origin.z = -15.0;

    //scene
    Scene scene;
    if (!scene.open(argv[1])) {
        std::cout << "error: could not open scene: " << argv[1] << std::endl;
        return 1; 
    }

    //intersection material, point and normal
    Material *material;
    Vec pt;
    Vec n;

    //create image and trace a ray for each pixel
    Image i(width, height);
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) { 

            //calculate ray direction vector
            r.direction.x = (double)x/width - 0.5;
            r.direction.y = -((double)y/height - 0.5); 
            r.direction.z = 1.0;

            if (scene.intersect(r, pt, n, material)) { 
                double r, g, b;
                material->shade(scene, pt, n, r, g, b); 
                i.set(x, y, r, g, b); 
            } else { 
                //dopey background
                i.set(x, y, (double)y/1024.0, (double)y/1024.0, (double)y/1024.0);
            }
        }
    }

    i.save("image.png");

    return 0;
}
