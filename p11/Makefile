###########################MAKEFILE#########################
all:pingpong
pingpong: queue.o pingpong.o pingpong-barrier.o
	gcc -o pingpong queue.o pingpong.o pingpong-barrier.o

queue.o: queue.c
	gcc -c queue.c -Wall

pingpong.o: pingpong.c
	gcc -c pingpong.c -Wall

pingpong-barrier.o: pingpong-barrier.c
	gcc -c pingpong-barrier.c -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf pingpong
