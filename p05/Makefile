###########################MAKEFILE#########################
all:pingpong
pingpong: queue.o pingpong.o pingpong-scheduler.o
	gcc -o pingpong queue.o pingpong.o pingpong-scheduler.o

queue.o: queue.c
	gcc -c queue.c -Wall

pingpong.o: pingpong.c
	gcc -c pingpong.c -Wall

pingpong-scheduler.o: pingpong-scheduler.c
	gcc -c pingpong-scheduler.c -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf pingpong
