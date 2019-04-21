rc:
	gcc -Wall -o main main.c `sdl-config --cflags --libs` -I./lib/include -L./libnm.a
	./main test.txt

compile:
	gcc -Wall -o main main.c `sdl-config --cflags --libs` -I./lib/include -L./libnm.a

memcheck:
	gcc -Wall -o main main.c `sdl-config --cflags --libs` -I./lib/include -L./libnm.a
	sudo valgrind --track-origins=yes ./main test.txt 

debug:
	gcc -ggdb -Wall -o main main.c `sdl-config --cflags --libs` -I./lib/include -L./libnm.a
	sudo gdb main
