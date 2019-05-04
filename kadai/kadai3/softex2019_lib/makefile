CC = gcc
SRC = main.c
FUNCS1 = read_database.c question.c answer.c
OBJS1 = main.o read_database.o question.o answer.o
LFLAG = -lm

main: $(OBJS1)

	$(CC) -o $@ $(OBJS1) $(LFLAG)

.c.o: $(SRC) $(FUNCS1)

	$(CC) -c $(SRC) $(FUNCS1)
