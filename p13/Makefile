###########################MAKEFILE#########################
all:pingpong
pingpong: queue.o pingpong.o harddisk.o pingpong-disco.o
	g++ -o pingpong queue.o pingpong.o harddisk.o pingpong-disco.o -lrt

queue.o: queue.c
	gcc -c queue.c -Wall

pingpong.o: pingpong.c
	gcc -c pingpong.c -Wall

harddisk.o: harddisk.c
	gcc -c harddisk.c -Wall

pingpong-disco.o: pingpong-disco.c
	gcc -c pingpong-disco.c -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf pingpong
