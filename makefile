all: debug

debug: main.o glad.o voronoi.o structs.o
	gcc -Wall -g -o build/debug build/*.o -lglfw -lm

main.o:
	gcc -c -o build/main.o src/main.c -lglfw

glad.o:
	gcc -c -o build/glad.o src/glad/glad.c

voronoi.o:
	gcc -c -o build/voronoi.o src/voronoi.c -lm

structs.o:
	gcc -c -o build/structs.o src/structs.c

pq.o:
	gcc -c -o build/pq.o src/priority_queue/pq.c
