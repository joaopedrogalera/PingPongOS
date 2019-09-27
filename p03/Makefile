###########################MAKEFILE#########################
all:pingpong
pingpong: queue.o pingpong.o pingpong-dispatcher.o
	gcc -o pingpong queue.o pingpong.o pingpong-dispatcher.o

queue.o: queue.c
	gcc -c queue.c -Wall

pingpong.o: pingpong.c
	gcc -c pingpong.c -Wall

pingpong-dispatcher.o: pingpong-dispatcher.c
	gcc -c pingpong-dispatcher.c -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf pingpong
