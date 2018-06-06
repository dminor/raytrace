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
    : photons(nullptr), map(nullptr), number_emitted(0)
{
}

void PhotonMap::build(const Scene &scene, int nphotons,
    bool include_direct_lighting, int max_depth)
{
    photons.reset(new Photon[nphotons]);
    this->nphotons = nphotons;

    //assume one light per scene for now
    Light *light = scene.lights.begin()->get();

    int i = 0;
    while (i < nphotons) {

        //initialize ray from light source
        Ray ray = light->emit();
        bool in_scene = true;
        float R, G, B;
        R = light->r;
        G = light->g;
        B = light->b;

        while (in_scene && ray.depth < max_depth) {
            Vec pt, n;
            Material *material;

            ++number_emitted;

            if (scene.intersect(ray, 0.1, std::numeric_limits<double>::max(),
                pt, n, material)) {

                //if lambertian material, store in photon map
                if (material->isLambertian()) {

                    LambertianMaterial *lm;
                    lm = static_cast<LambertianMaterial *>(material);

                    Vec direction = ray.origin - pt;
                    direction.normalize();
                    float c = n.dot(direction);
                    if (c > 0.0f) {
                        R *= lm->r*c;
                        G *= lm->g*c;
                        B *= lm->b*c;
                    }

                    /*
                    printf("%d %f %f %f -> ", ray.depth, lm->r, lm->g, lm->b);
                    printf("%f %f %f\n", R, G, B);
                    */

                    if (include_direct_lighting || ray.depth > 0) {
                        photons[i].direction = direction;
                        photons[i].location = pt;
                        photons[i].r = R;
                        photons[i].g = G;
                        photons[i].b = B;

                        ++i;
                    }

                    //Attenuation due to surface absorption
                    R *= lm->reflectivity;
                    G *= lm->reflectivity;
                    B *= lm->reflectivity;

                }  else {
                }

                if (R < 0.001 && G < 0.001 && B < 0.001) {
                    in_scene = false;
                } else {
                    //update ray
                    ++ray.depth;
                    ray.origin = pt;

                    Vec u, v;
                    n.construct_basis(u, v);
                    Vec w = Vec::sample_hemisphere_cosine_weighted();
                    ray.direction = u*w.x + v*w.y + n*w.z;
                    ray.direction.normalize();
                }
            } else {
                in_scene = false;
            }
        }
    }

    map.reset(new KdTree<Photon, double>(3, photons.get(), nphotons));
}

void PhotonMap::query(const Vec &pt, const Vec &norm, int nphotons, double eps,
    float &r, float &g, float &b) const
{
    r = g = b = 0.0f;

    std::list<std::pair<Photon *, double> > qr = map->knn(nphotons, pt, eps);
    for (std::list<std::pair<Photon *, double> >::iterator itor = qr.begin();
        itor != qr.end(); ++itor) {
            if (itor->first->direction.dot(norm) > 0) {
                r += itor->first->r;
                g += itor->first->g;
                b += itor->first->b;
            }
    }

    double scale = 1.0/(3.14159265358979323f*qr.back().second*number_emitted);

    r *= scale;
    g *= scale;
    b *= scale;

}

void PhotonMap::write(const char *filename) const
{
    FILE *f = fopen(filename, "w");

    if (!f) {
        fprintf(stderr, "error: could not write photon map to %s\n", filename);
    }

    fprintf(f, "%d\n", nphotons);

    for (int i = 0; i < nphotons; ++i) {
        const Photon &p = photons[i];

        fprintf(f, "%f %f %f %f %f %f\n",
            p.location.x, p.location.y, p.location.z, p.r, p.g, p.b);
    }

    fclose(f);
}
