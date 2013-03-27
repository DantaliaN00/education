
CC = g++
FLAGS = -g -DGL_GLEXT_PROTOTYPES -I./glm
LDFLAGS = -lX11 -lpthread


all: main

main: main.o raytrace.o parse.o geometry.o util.h light.o accel.o Box.o
	$(CC) $(FLAGS) -o raytracer main.o raytrace.o parse.o geometry.o light.o accel.o Box.o $(LDFLAGS)

main.o: main.cpp util.h
	$(CC) $(FLAGS) -c main.cpp

raytrace.o: raytrace.cpp Ray.h util.h accel.o
	$(CC) $(FLAGS) -c raytrace.cpp

geometry.o: geometry.cpp util.h
	$(CC) $(FLAGS) -c geometry.cpp

accel.o: accel.cpp
	$(CC) $(FLAGS) -c accel.cpp

parse.o: parse.cpp parse.h geometry.h light.h util.h
	$(CC) $(FLAGS) -c parse.cpp

light.o: light.cpp
	$(CC) $(FLAGS) -c light.cpp

testRaytrace.o:
	$(CC) $(FLAGS) -c testRaytrace.cpp raytrace.h Ray.h

Box.o: geometry.h Ray.h
	$(CC) $(FLAGS) -c Box.cpp

test: testRaytrace.o raytrace.o parse.o geometry.o util.h
	$(CC) $(FLAGS) -o testRaytracer testRaytrace.o raytrace.o parse.o geometry.o $(LDFLAGS)

clean:
	rm -rf raytracer *.o *.gch *. *\~

BoxTest.o: Box.h Ray.h geometry.h Box.cpp
	$(CC) $(FLAGS) -c BoxTest.cpp

boxtest: BoxTest.o Box.o geometry.o
	$(CC) $(FLAGS) -o boxtest BoxTest.o Box.o geometry.o

run:
	./raytracer
