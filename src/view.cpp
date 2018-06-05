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

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

#include "lua_functions.h"
#include "view.h"

static int eyepoint(lua_State *ls)
{
    if (!lua_istable(ls, -1)) {
        luaL_error(ls, "eyepoint: expected table");
    }

    //get reference to the scene we are building
    lua_getglobal(ls, "VIEW");
    View *view = reinterpret_cast<View *>(lua_touserdata(ls, -1));
    lua_pop(ls, 1);

    //eyepoint
    lua_getfield(ls, -1, "pos");
    get_xyz(ls, view->pos.x,view->pos.y, view->pos.z);
    lua_pop(ls, 1);

    //direction
    lua_getfield(ls, -1, "dir");
    get_xyz(ls, view->dir.x, view->dir.y, view->dir.z);
    lua_pop(ls, 1);

    //up
    lua_getfield(ls, -1, "up");
    get_xyz(ls, view->up.x, view->up.y, view->up.z);
    lua_pop(ls, 1);

    return 0;
}

static int image(lua_State *ls)
{
    if (!lua_istable(ls, -1)) {
        luaL_error(ls, "image: expected table");
    }

    //get reference to the scene we are building
    lua_getglobal(ls, "VIEW");
    View *view = reinterpret_cast<View *>(lua_touserdata(ls, -1));
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "width");
    view->width = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "height");
    view->height = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);

    return 0;
}

static int surface(lua_State *ls)
{
    if (!lua_istable(ls, -1)) {
        luaL_error(ls, "surface : expected table");
    }

    //get reference to the scene we are building
    lua_getglobal(ls, "VIEW");
    View *view = reinterpret_cast<View *>(lua_touserdata(ls, -1));
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "u0");
    view->u0 = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "v0");
    view->v0 = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "u1");
    view->u1 = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "v1");
    view->v1 = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);

    return 0;
}

static luaL_Reg fns[] = {
    {"eyepoint", eyepoint},
    {"image", image},
    {"surface", surface},
};

View::~View()
{
}

bool View::open(const char *filename)
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

    //push reference to view so that we can modify it
    lua_pushlightuserdata(ls, this);
    lua_setglobal(ls, "VIEW");

    //attempt to run view definition file
    if (luaL_dofile(ls, filename)) {
        const char *err = lua_tostring(ls, -1);
        if (err) fprintf(stderr, "%s\n", err);
        result = false;
    }

    lua_close(ls);
    return result;
}
