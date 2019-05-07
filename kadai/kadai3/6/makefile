TARGET=main
CC=gcc
AR=ar
AROPS=rcs
MAIN=main.c
SRCS=read_database.c question.c answer.c
LIB=lib.a
OBJS=read_database.o question.o answer.o

all: $(LIB) $(TARGET)

$(TARGET): $(LIB)
	$(CC) -o $@ $(MAIN) $^

$(LIB): $(OBJS)
	$(AR) $(AROPS) $@ $^

.c.o:
	$(CC) -c $<

clean:
	rm -rf *.a *.o *~ $(TARGET)

