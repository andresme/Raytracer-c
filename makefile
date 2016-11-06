LDLIBS= -lm -g -fopenmp
CHECK=-Wall -Wextra

raytracer: raytracer.o sphere.o load.o polygon.o cylinder.o cone.o structs.o disc.o quadric.o
	cc $(CHECK) $(LDLIBS) -o $@ raytracer.o sphere.o load.o polygon.o cylinder.o cone.o structs.o disc.o quadric.o -lm

raytracer.o: raytracer.c raytracer.h
	cc -c -g -o $@ $<

disc.o: disc.c
	cc -c -g -o $@ $<

quadric.o: quadric.c
	cc -c -g -o $@ $<

sphere.o: sphere.c
	cc -c -g -o $@ $<

polygon.o: polygon.c
	cc -c -g -o $@ $<

cylinder.o: cylinder.c
	cc -c -g -o $@ $<

cone.o: cone.c
	cc -c -g -o $@ $<

load.o: load.c
	cc -c -g -o $@ $<

structs.o: structs.c
	cc -c -g -o $@ $<

clean:
	rm -f raytracer raytracer.o sphere.o load.o polygon.o cylinder.o cone.o structs.o disc.o quadric.o

compile:
	rm -f raytracer raytracer.o sphere.o load.o polygon.o cylinder.o cone.o structs.o disc.o quadric.o

	cc -c -o raytracer.o raytracer.c
	cc -c -o sphere.o sphere.c 
	cc -c -o load.o load.c
	cc -c -o polygon.o polygon.c
	cc -c -o cylinder.o cylinder.c
	cc -c -o cone.o cone.c
	cc -c -o structs.o structs.c
	cc -c -o disc.o disc.c
	cc -c -o quadric.o
	cc $(CHECK) $(LDLIBS) -o $@ raytracer.o sphere.o load.o polygon.o cylinder.o cone.o structs.o disc.o quadric.o

gdb: raytracer.o sphere.o polygon.o cylinder.o cone.o structs.o disc.o quadric.o
	cc $(CHECK) $(LDLIBS) -o $@ rayTracer.o sphere.o load.o polygon.o cylinder.o cone.o structs.o disc.o quadric.o
	gdb ./rayTracer
