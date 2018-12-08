INCS = -I/usr/include/lua5.2
LIBS = -lpng -llua5.2
CFLAGS = -g -O2 -Wall
LDFLAGS = -pthread
OBJS = lua_functions.o image.o photon_map.o scene.o raytrace.o view.o
TARGET = ../bin/raytrace

all: $(OBJS)
	mkdir -p ../bin
	g++ $(LDFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

.cpp.o:
	g++ $(INCS) $(CFLAGS) -c $< -o $@


image.o: image.h

photon_map.o: photon_map.h ray.h vec.h

scene.o: dielectric_material.h lambertian_material.h specular_material.h

raytrace.o: dielectric_material.h group.h intersectable.h plane.h quat.h ray.h\
            sphere.h triangle_mesh.h vec.h view.h lambertian_material.h\
            specular_material.h

view.o: view.h

clean:
	rm *.o $(TARGET)
