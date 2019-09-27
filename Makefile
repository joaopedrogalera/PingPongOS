###########################MAKEFILE#########################
all:queue
queue: queue.o testafila.o
	gcc -o queue queue.o testafila.o

queue.o: queue.c
	gcc -c queue.c -Wall

testafila.o: testafila.c
	gcc -c testafila.c -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf queue
