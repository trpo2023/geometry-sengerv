all: main

main: main1.c
	gcc -Wall -Werror main1.c -o main -lm

