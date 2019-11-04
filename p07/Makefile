###########################MAKEFILE#########################
all:pingpong
pingpong: queue.o pingpong.o pingpong-maintask.o
	gcc -o pingpong queue.o pingpong.o pingpong-maintask.o

queue.o: queue.c
	gcc -c queue.c -Wall

pingpong.o: pingpong.c
	gcc -c pingpong.c -Wall

pingpong-maintask.o: pingpong-maintask.c
	gcc -c pingpong-maintask.c -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf pingpong
