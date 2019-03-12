.PHONY: all lib debug

all: lib

lib:
	gcc -g -c -fPIC *.c
	ar rcs libnm.a *.o

debug:
	gcc -g -o prog main.c -L. -lnm
