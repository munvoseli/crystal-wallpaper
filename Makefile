all:
	gcc -o crystal crystal.c
rend:
	gcc -o render `pkg-config --cflags imlib2 x11` render.c `pkg-config --libs imlib2 x11`
comp:
	gcc -g -o composite `pkg-config --cflags imlib2 x11` crystal-auto.c `pkg-config --libs imlib2 x11`
