all: main.o

main.o: main.c
	gcc -Wall main.c -o app -lm
run:
	./app example
