OBJS = sdl2_mutex.c
OBJ_NAME = sdl2_mute

all: $(OBJS)
	gcc $(OBJS) -lm -lSDL2 -lSDL2_gfx -lcwiimote -L/usr/lib -g -o $(OBJ_NAME)

.PHONY: clean

clean:
	rm -f *.o core $(OBJ_NAME)
