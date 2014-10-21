
INCS =
LIBS = -lSDL -lGL -lGLEW -lGLU
CFLAGS =
LDFLAGS =
OBJS = main.o
TARGET = ../../bin/view-photon-map

all: $(OBJS)
	g++ $(LDFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

.cpp.o:
	g++ $(INCS) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(TARGET)
