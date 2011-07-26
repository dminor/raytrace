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


extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

#include <cstdio>

#include "dielectric_material.h"
#include "lambertian_material.h"
#include "material.h"
#include "plane.h"
#include "point_light.h"
#include "quat.h"
#include "rectangular_light.h"
#include "scene.h"
#include "specular_material.h"
#include "sphere.h"
#include "transform.h"
#include "triangle_mesh.h"

#include "lua_functions.h"

static int dielectric(lua_State *ls)
{
    if (!lua_istable(ls, -1)) { 
        luaL_error(ls, "dielectric: expected table");
    }
 
    lua_getfield(ls, -1, "nt");
    double nt = luaL_checknumber(ls, -1); 
    lua_pop(ls, 1);

    DielectricMaterial *mat = new DielectricMaterial;
    mat->nt = nt;
    lua_pushlightuserdata(ls, mat);

    return 1;
}
static int group(lua_State *ls)
{
    if (!lua_istable(ls, -1)) { 
        luaL_error(ls, "group: expected table");
    }

    Group *group = new Group;

    lua_getfield(ls, -1, "children");
    lua_pushnil(ls); 
    while (lua_next(ls, -2)) {
        Intersectable *i = reinterpret_cast<Intersectable *>(lua_touserdata(ls, -1));
        lua_pop(ls, 1);
        group->children.push_back(i);
    }
    lua_pop(ls, 1);

    lua_pushlightuserdata(ls, group);
    return 1;
}

static int lambertian(lua_State *ls)
{
    if (!lua_istable(ls, -1)) { 
        luaL_error(ls, "lambertian: expected table");
    }

    double r, g, b;

    lua_getfield(ls, -1, "r");
    r = luaL_checknumber(ls, -1); 
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "g");
    g = luaL_checknumber(ls, -1); 
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "b");
    b = luaL_checknumber(ls, -1); 
    lua_pop(ls, 1);
 
    LambertianMaterial *mat = new LambertianMaterial;
    mat->r = r; mat->g = g; mat->b = b; 
    lua_pushlightuserdata(ls, mat);

    return 1;
}

static int pointlight(lua_State *ls)
{
    if (!lua_istable(ls, -1)) { 
        luaL_error(ls, "pointlight: expected table");
    }

    lua_getfield(ls, -1, "radiance");
    double r, g, b; 
    get_rgb(ls, r, g, b);    
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "location");
    double x, y, z;
    get_xyz(ls, x, y, z);    

    PointLight *light = new PointLight;
    light->r = r; light->g = g; light->b = b;
    light->location.x = x; light->location.y = y; light->location.z = z;
    lua_pushlightuserdata(ls, light);

    return 1;
}

static int plane(lua_State *ls)
{
    if (!lua_istable(ls, -1)) { 
        luaL_error(ls, "plane: expected table");
    }

    double pt_x, pt_y, pt_z;
    lua_getfield(ls, -1, "pt");
    get_xyz(ls, pt_x, pt_y, pt_z);    
    lua_pop(ls, 1);

    double norm_x, norm_y, norm_z;
    lua_getfield(ls, -1, "norm");
    get_xyz(ls, norm_x, norm_y, norm_z);    
    lua_pop(ls, 1);


    lua_getfield(ls, -1, "material");
    Material *mat = reinterpret_cast<Material *>(lua_touserdata(ls, -1));
    lua_pop(ls, 1);

    Plane *plane = new Plane;
    plane->p.x = pt_x; plane->p.y = pt_y; plane->p.z = pt_z;
    plane->normal.x = norm_x; plane->normal.y = norm_y; plane->normal.z = norm_z;
    plane->material = mat;
    lua_pushlightuserdata(ls, plane);

    return 1;
}

static int quat(lua_State *ls)
{
    if (!lua_istable(ls, -1)) { 
        luaL_error(ls, "quat: expected table");
    }

    lua_getfield(ls, -1, "angle");
    double angle = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);

    double x, y, z;
    get_xyz(ls, x, y, z);    

    Quat *quat = new Quat(angle, x, y, z);
    lua_pushlightuserdata(ls, quat);

    return 1;
}

static int rectangularlight(lua_State *ls)
{
    if (!lua_istable(ls, -1)) { 
        luaL_error(ls, "rectangularlight: expected table");
    }

    lua_getfield(ls, -1, "radiance");
    double r, g, b; 
    get_rgb(ls, r, g, b);    
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "pt");
    double x1, y1, z1;
    get_xyz(ls, x1, y1, z1);    
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "normal");
    double x2, y2, z2;
    get_xyz(ls, x2, y2, z2);    
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "width");
    double width = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "height");
    double height = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);

    RectangularLight *light = new RectangularLight;
    light->r = r; light->g = g; light->b = b;
    light->pt.x = x1; light->pt.y = y1; light->pt.z = z1;
    light->normal.x = x2; light->normal.y = y2; light->normal.z = z2;
    light->width = width;
    light->height = height;
    lua_pushlightuserdata(ls, light);

    return 1;
}
static int scene(lua_State *ls)
{
    if (!lua_istable(ls, -1)) { 
        luaL_error(ls, "scene: expected table");
    }

    //get reference to the scene we are building
    lua_getglobal(ls, "SCENE");
    Scene *scene = reinterpret_cast<Scene *>(lua_touserdata(ls, -1));
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "lights");
    lua_pushnil(ls); 
    while (lua_next(ls, -2)) {
        Light *light = reinterpret_cast<Light *>(lua_touserdata(ls, -1));
        lua_pop(ls, 1);
        scene->lights.push_back(light);
    }
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "children");
    lua_pushnil(ls); 
    while (lua_next(ls, -2)) {
        Intersectable *i = reinterpret_cast<Intersectable *>(lua_touserdata(ls, -1));
        lua_pop(ls, 1);
        scene->children.push_back(i); 
    }
    lua_pop(ls, 1);

    return 0;
}

static int specular(lua_State *ls)
{
    if (!lua_istable(ls, -1)) { 
        luaL_error(ls, "specular: expected table");
    }
 
    SpecularMaterial *mat = new SpecularMaterial;
    lua_pushlightuserdata(ls, mat);

    return 1;
}


static int sphere(lua_State *ls)
{
    if (!lua_istable(ls, -1)) { 
        luaL_error(ls, "sphere: expected table");
    }

    double x, y, z;
    lua_getfield(ls, -1, "centre");
    get_xyz(ls, x, y, z);    
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "radius");
    double radius = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "material");
    Material *mat = reinterpret_cast<Material *>(lua_touserdata(ls, -1));
    lua_pop(ls, 1);

    Sphere *sphere = new Sphere;
    sphere->centre.x = x; sphere->centre.y = y; sphere->centre.z = z;
    sphere->radius = radius;
    sphere->material = mat;
    lua_pushlightuserdata(ls, sphere);

    return 1;
}

static int transform(lua_State *ls)
{
    if (!lua_istable(ls, -1)) { 
        luaL_error(ls, "transform: expected table");
    }

    double x, y, z;
    lua_getfield(ls, -1, "translation");
    get_xyz(ls, x, y, z);
    lua_pop(ls, 1);

    //rotations -- we loop over these and multiply them together
    Quat rotation;
    lua_getfield(ls, -1, "rotation");
    lua_pushnil(ls); 
    while (lua_next(ls, -2)) {
        Quat *q = reinterpret_cast<Quat *>(lua_touserdata(ls, -1));
        lua_pop(ls, 1);
        rotation = rotation * *q;
        delete(q);
    }
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "child");
    Intersectable *child = reinterpret_cast<Intersectable *>(lua_touserdata(ls, -1)); 
    lua_pop(ls, 1);

    Transform *transform = new Transform;
    transform->translation.x = x; transform->translation.y = y; transform->translation.z = z;
    transform->rotation = rotation; 
    transform->child = child;

    lua_pushlightuserdata(ls, transform);

    return 1;
}


static int trimesh(lua_State *ls)
{
    if (!lua_istable(ls, -1)) { 
        luaL_error(ls, "trimesh: expected table");
    }

    lua_getfield(ls, -1, "filename");
    const char *filename = luaL_checkstring(ls, -1);
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "material");
    Material *mat = reinterpret_cast<Material *>(lua_touserdata(ls, -1));
    lua_pop(ls, 1);

    TriangleMesh *obj = new TriangleMesh;
    obj->open(filename);
    obj->material = mat;

    lua_pushlightuserdata(ls, obj);

    return 1;
}

static luaL_Reg fns[] = {
    {"dielectric", dielectric},
    {"group", group},
    {"lambertian", lambertian},
    {"pointlight", pointlight},
    {"plane", plane},
    {"quat", quat},
    {"rectangularlight", rectangularlight},
    {"scene", scene},
    {"specular", specular},
    {"sphere", sphere},
    {"transform", transform},
    {"trimesh", trimesh},
    {0, 0}
};

Scene::~Scene() { 
    for (std::vector<Light *>::iterator itor = lights.begin(); itor != lights.end(); ++itor) {
        delete *itor; 
    } 
}

bool Scene::open(const char *filename)
{
    bool result = true;
 
    lua_State *ls = luaL_newstate(); 
    luaL_openlibs(ls);

    //register functions in global namespace
    luaL_Reg *fn = fns;
    while (fn->name != 0) {
        lua_register(ls, fn->name, fn->func); 
        ++fn;
    }

    //push reference to scene so that we can modify it
    lua_pushlightuserdata(ls, this);
    lua_setglobal(ls, "SCENE");

    //attempt to run scene definition file
    if (luaL_dofile(ls, filename)) {
        const char *err = lua_tostring(ls, -1);
        if (err) fprintf(stderr, "%s\n", err);
        result = false;
    }

    lua_close(ls);
    return result;
}
