
INCS = -I../../include 
LIBS = -lpng 
CFLAGS = -g
LDFLAGS = 
OBJS = raytrace.o image.o
TARGET = ../bin/raytrace

all: $(OBJS)
	g++ $(LDFLAGS) $(LIBS) $(OBJS) -o $(TARGET) 

.cpp.o:
	g++ $(INCS) $(CFLAGS) -c $< -o $@

raytrace.o: intersectable.h plane.h quat.h ray.h sphere.h triangle_mesh.h vec.h

image.o: image.h

clean:
	rm *.o $(TARGET) 
