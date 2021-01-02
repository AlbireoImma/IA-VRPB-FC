build:
	g++ -Wall -O -o FC-VRPB.o FC-VRPB.cpp

run:
	./FC-VRPB.o ./instancias/$(file).txt > ./Resultados/$(file).out

timeit:
	/usr/bin/time --format="Tiempo %e" ./FC-VRPB.o ./instancias/$(file)

clean:
	rm -f *.o