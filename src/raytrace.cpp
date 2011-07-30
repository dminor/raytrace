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

#include <cstdio>
#include <limits>

#include "image.h" 
#include "photon_map.h"
#include "scene.h"
#include "vec.h"
#include "view.h"

int main(int argc, char **argv)
{ 

    if (argc < 3) {
        fprintf(stderr, "usage: raytrace <view> <scene> [--samples]");
        fprintf(stderr, " [--use-photon-map] [--photons]");
        return 1;
    }

    //view
    View view;
    if (!view.open(argv[1])) {
        fprintf(stderr, "error: could not open view: %s\n",argv[1]);
        return 1; 
    }

    //scene
    Scene scene;
    if (!scene.open(argv[2])) {
        fprintf(stderr, "error: could not open scene: %s\n",argv[2]);
        return 1; 
    }

    //look at other arguments
    scene.use_photon_map = false;
    int samples = 1;
    int bphotons = 10000;
    int qphotons = 50;

    for (int i = 3; i < argc; ++i) {
        if (sscanf(argv[i], "--samples=%d", &samples) == 1) { 
            if (samples < 1) samples = 1;
        }

        if (!strcmp(argv[i], "--use-photon-map")) { 
            scene.use_photon_map = true;
        }

        if (sscanf(argv[i], "--build-photons=%d", &bphotons) == 1) { 
            if (bphotons < 1) bphotons = 1;
        }

        if (sscanf(argv[i], "--query-photons=%d", &qphotons) == 1) { 
            if (qphotons < 1) qphotons = 1;
        }
    }

    //build photon map
    if (scene.use_photon_map) { 
        scene.photon_map.build(scene, bphotons, true, 5);  
        scene.query_photons = qphotons;
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
        printf("%.1f percent complete\n", 100.0*(double)x/(double)view.width);

        for (int y = 0; y < view.height; ++y) { 
            double R = 0.0, G = 0.0, B = 0.0;

            for (int s = 0; s < samples; ++s) { 
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

                    double scale = 1.0/(double)samples;

                    R += r*scale; 
                    G += g*scale;
                    B += b*scale;
                }
            }

            i.set(x, y, R, G, B); 
        }
    }

    i.save("image.png");

    return 0;
}
