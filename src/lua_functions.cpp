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

#include "lua_functions.h"

extern "C" {
    #include <lauxlib.h>
    #include <lualib.h>
}


// helper function to get x, y and z values from table at top of stack
void get_xyz(lua_State *ls, double &x, double &y, double &z)
{
    lua_getfield(ls, -1, "x");
    x = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "y");
    y = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "z");
    z = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);
}

// helper function to get r, g and b values from table at top of stack
void get_rgb(lua_State *ls, double &r, double &g, double &b)
{
    lua_getfield(ls, -1, "r");
    r = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "g");
    g = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);

    lua_getfield(ls, -1, "b");
    b = luaL_checknumber(ls, -1);
    lua_pop(ls, 1);
}
