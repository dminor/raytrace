
INCS = -I../../include 
LIBS = -lpng 
CFLAGS = -g
LDFLAGS = 
OBJS = raytrace.o image.o scene.o
TARGET = ../bin/raytrace

all: $(OBJS)
	g++ $(LDFLAGS) $(LIBS) $(OBJS) -o $(TARGET) 

.cpp.o:
	g++ $(INCS) $(CFLAGS) -c $< -o $@


image.o: image.h

scene.o: scene.h

raytrace.o: group.h intersectable.h plane.h quat.h ray.h sphere.h triangle_mesh.h vec.h \
            lambertian_material.h

clean:
	rm *.o $(TARGET) 
