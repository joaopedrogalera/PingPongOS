###########################MAKEFILE#########################
all:pingpong
pingpong: queue.o pingpong.o pingpong-join.o
	gcc -o pingpong queue.o pingpong.o pingpong-join.o

queue.o: queue.c
	gcc -c queue.c -Wall

pingpong.o: pingpong.c
	gcc -c pingpong.c -Wall

pingpong-join.o: pingpong-join.c
	gcc -c pingpong-join.c -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf pingpong
