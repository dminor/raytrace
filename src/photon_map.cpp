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

#include <cstdio>

#include "lambertian_material.h"
#include "photon_map.h"
#include "ray.h"

PhotonMap::PhotonMap()
    : photons(0), map(0), number_emitted(0)
{ 
}

PhotonMap::~PhotonMap()
{
    if (map) delete map;
    if (photons) delete[] photons;
}

void PhotonMap::build(const Scene &scene, int nphotons,
    bool include_direct_lighting, int max_depth)
{
    photons = new Photon[nphotons];

    Light *light;

    //assume one light per scene for now 
    for (std::vector<Light *>::const_iterator itor = scene.lights.begin(); itor != scene.lights.end(); ++itor) {
        light = *itor;
    } 

    int i = 0;
    while (i < nphotons) {

        //initialize ray from light source
        Ray ray = light->emit();
        bool in_scene = true; 
        double R, G, B;
        R = light->r*10000.0/(double)nphotons;
        G = light->g*10000.0/(double)nphotons;
        B = light->b*10000.0/(double)nphotons;

        ++number_emitted;

        while (in_scene && ray.depth < max_depth) { 
            Vec pt, n; 
            Material *material;

            if (scene.intersect(ray, 0.1, std::numeric_limits<double>::max(),
                pt, n, material)) {

                Vec direction = ray.origin - pt;
                double length = direction.magnitude();
                double attenuation = 1.0 / (length*length); 
                if (attenuation > 1.0) attenuation = 1.0;

                R *= attenuation;
                G *= attenuation;
                B *= attenuation; 

                //if lambertian material, store in photon map
                if (material->isLambertian()) { 

                    LambertianMaterial *lm;
                    lm = static_cast<LambertianMaterial *>(material);

                    direction.normalize();
                    double c = n.dot(direction);
                    if (c < 0.0) c = 0.0;
                    if (c > 1.0) c = 1.0;

                    R *= lm->r*c;
                    G *= lm->g*c;
                    B *= lm->b*c;

                    photons[i].location = pt;
                    photons[i].r = R;
                    photons[i].g = G;
                    photons[i].b = B;

                    //Attenuation due to surface absorption
                    R *= lm->reflectivity;
                    G *= lm->reflectivity;
                    B *= lm->reflectivity; 

                    ++i;
                }  else {
                }

                if (R < 0.0001 && G < 0.001 && B < 0.001) {
                    in_scene = false;
                } else { 
                    //update ray 
                    ++ray.depth;
                    ray.origin = pt;

                    Vec u, v;
                    n.construct_basis(u, v); 
                    Vec w = Vec::random_cosine_vec(); 
                    ray.direction = u*w.x + v*w.y + n*w.z; 
                }
            } else { 
                in_scene = false;
            } 
        }
    }

    map = new KdTree<Photon>(3, photons, nphotons); 
}

void PhotonMap::query(const Vec &pt, int nphotons, double eps,
    double &r, double &g, double &b) const
{
    r = g = b = 0.0;
 
    std::list<std::pair<Photon *, double> > qr = map->knn(nphotons, pt, eps);
    for (std::list<std::pair<Photon *, double> >::iterator itor = qr.begin();
        itor != qr.end(); ++itor) {
            r += itor->first->r;
            g += itor->first->g;
            b += itor->first->b;
    }

    double scale = qr.back().second;
    if (scale > 0.0) {
        scale = 1.0/(3.14159265358979323*scale*scale*number_emitted);
    }

    r *= scale;
    g *= scale;
    b *= scale;
}


