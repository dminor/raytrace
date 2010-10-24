
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

clean:
	rm *.o $(TARGET) 
