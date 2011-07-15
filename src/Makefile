
INCS = -I../../include -I/usr/include/lua5.1
LIBS = -lpng -llua5.1
CFLAGS = -g
LDFLAGS = 
OBJS = lua_functions.o raytrace.o image.o scene.o view.o
TARGET = ../bin/raytrace

all: $(OBJS)
	g++ $(LDFLAGS) $(LIBS) $(OBJS) -o $(TARGET) 

.cpp.o:
	g++ $(INCS) $(CFLAGS) -c $< -o $@


image.o: image.h

scene.o: scene.h lambertian_material.h specular_material.h

raytrace.o: group.h intersectable.h plane.h quat.h ray.h sphere.h\
            triangle_mesh.h vec.h view.h lambertian_material.h light.h\
            point_light.h specular_material.h

view.o: view.h

clean:
	rm *.o $(TARGET) 
