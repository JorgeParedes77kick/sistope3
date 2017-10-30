all: wave.c
	gcc -o wave wave.c -lpthread -Wall -I.