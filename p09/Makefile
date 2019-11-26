###########################MAKEFILE#########################
all:pingpong
pingpong: queue.o pingpong.o pingpong-sleep.o
	gcc -o pingpong queue.o pingpong.o pingpong-sleep.o

queue.o: queue.c
	gcc -c queue.c -Wall

pingpong.o: pingpong.c
	gcc -c pingpong.c -Wall

pingpong-sleep.o: pingpong-sleep.c
	gcc -c pingpong-sleep.c -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf pingpong
