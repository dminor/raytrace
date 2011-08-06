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

#include <cstdio>

void clamp(float &v, float min, float max)
{
    if (v < min) v = min;
    if (v > max) v = max;
}

Image::Image(size_t width, size_t height) : width(width), height(height)
{
    rows = new png_bytep[height];
    for (size_t i = 0; i < height; ++i) {
        rows[i] = new png_byte[width*3];
    } 
}

Image::~Image()
{ 
    for (size_t i = 0; i < height; ++i) {
        delete[] rows[i];
    }

    delete[] rows; 
}

void Image::set(size_t x, size_t y, float r, float g, float b)
{
    if (x < width && y < height) {
        clamp(r, 0.0, 1.0);
        clamp(g, 0.0, 1.0);
        clamp(b, 0.0, 1.0);

        rows[y][x*3] = (png_byte)(r * 255.0);
        rows[y][x*3 + 1] = (png_byte)(g * 255.0);
        rows[y][x*3 + 2] = (png_byte)(b * 255.0);
    } 
}

bool Image::save(const char *filename)
{
    FILE *f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "error: could not open: %s\n", filename);
        return false;
    }
    
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    png_infop info = png_create_info_struct(png);

    //set up error handler
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(f);
        fprintf(stderr, "error: could not write png.\n");
        return false;
    }

    png_init_io(png, f);
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png, info);
    png_write_image(png, rows);
    png_write_end(png, 0); 
    fclose(f);

    return true;
}
