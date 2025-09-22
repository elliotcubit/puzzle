build: main.c
	gcc -o main main.c

run: build
	./main

