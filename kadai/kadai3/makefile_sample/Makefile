TARGET=prog
CC=gcc
AR=ar
AROPS=rcs
MAIN=main.c
SRCS=func1.c func2.c
LIB=lib.a
OBJS=func1.o func2.o

all: $(LIB) $(TARGET)

$(TARGET): $(LIB)
	$(CC) -o $@ $(MAIN) $^

$(LIB): $(OBJS)
	$(AR) $(AROPS) $@ $^

.c.o:
	$(CC) -c $<

clean:
	rm -rf *.a *.o *~ $(TARGET)

