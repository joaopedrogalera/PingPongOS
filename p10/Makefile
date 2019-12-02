###########################MAKEFILE#########################
all:pingpong-semaphore pingpong-racecond pingpong-prodcons
pingpong-semaphore: queue.o pingpong.o pingpong-semaphore.o
	gcc -o pingpong-semaphore queue.o pingpong.o pingpong-semaphore.o

pingpong-racecond: queue.o pingpong.o pingpong-racecond.o
	gcc -o pingpong-racecond queue.o pingpong.o pingpong-racecond.o

pingpong-prodcons: queue.o pingpong.o pingpong-prodcons.o
	gcc -o pingpong-prodcons queue.o pingpong.o pingpong-prodcons.o

queue.o: queue.c
	gcc -c queue.c -Wall

pingpong.o: pingpong.c
	gcc -c pingpong.c -Wall

pingpong-semaphore.o: pingpong-semaphore.c
	gcc -c pingpong-semaphore.c -Wall

pingpong-racecond.o: pingpong-racecond.c
	gcc -c pingpong-racecond.c -Wall

pingpong-prodcons.o: pingpong-prodcons.c
	gcc -c pingpong-prodcons.c -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf pingpong-semaphore pingpong-racecond pingpong-prodcons
