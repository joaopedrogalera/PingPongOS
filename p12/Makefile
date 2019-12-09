###########################MAKEFILE#########################
all:pingpong
pingpong: queue.o pingpong.o pingpong-mqueue.o
	g++ -o pingpong queue.o pingpong.o pingpong-mqueue.o

queue.o: queue.c
	gcc -c queue.c -Wall

pingpong.o: pingpong.c
	gcc -c pingpong.c -Wall

pingpong-mqueue.o: pingpong-mqueue.c
	gcc -c pingpong-mqueue.c -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf pingpong
