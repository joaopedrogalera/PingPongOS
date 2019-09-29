###########################MAKEFILE#########################
all:pingpong
pingpong: queue.o pingpong.o pingpong-preempcao.o
	gcc -o pingpong queue.o pingpong.o pingpong-preempcao.o

queue.o: queue.c
	gcc -c queue.c -Wall

pingpong.o: pingpong.c
	gcc -c pingpong.c -Wall

pingpong-scheduler.o: pingpong-preempcao.c
	gcc -c pingpong-preempcao.c -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf pingpong
