###########################MAKEFILE#########################
all:pingpong
pingpong: queue.o pingpong.o pingpong-contab-prio.o
	gcc -o pingpong queue.o pingpong.o pingpong-contab-prio.o

queue.o: queue.c
	gcc -c queue.c -Wall

pingpong.o: pingpong.c
	gcc -c pingpong.c -Wall

pingpong-contab-prio.o: pingpong-contab-prio.c
	gcc -c pingpong-contab-prio.c -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf pingpong
