###########################MAKEFILE#########################
all:pp1 pp2 pp3
pp1: pingpong.o pingpong-tasks1.o
	gcc -o pp1 pingpong.o pingpong-tasks1.o

pp2: pingpong.o pingpong-tasks2.o
	gcc -o pp2 pingpong.o pingpong-tasks2.o

pp3: pingpong.o pingpong-tasks3.o
	gcc -o pp3 pingpong.o pingpong-tasks3.o

pingpong.o: pingpong.c
	gcc -c pingpong.c -Wall

pingpong-tasks1.o: pingpong-tasks1.c
	gcc -c pingpong-tasks1.c -Wall

pingpong-tasks2.o: pingpong-tasks2.c
	gcc -c pingpong-tasks2.c -Wall

pingpong-tasks3.o: pingpong-tasks3.c
	gcc -c pingpong-tasks3.c -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf pp1 pp2 pp3
