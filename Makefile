all: main

main: main.c
	gcc -Wall -Werror main.c -o main -lm

