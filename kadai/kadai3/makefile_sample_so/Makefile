TARGET=prog
CC=gcc
AR=ar
AROPS=rcs
MAIN=main.c
SRCS=func1.c func2.c
LIBNAME=func

all: $(LIBNAME) $(TARGET)


$(TARGET): lib$(LIBNAME).so
	$(CC) -I./ -L./ -o $@ $(MAIN) -l$(LIBNAME)

$(LIBNAME): $(SRCS)
	$(CC) -shared -fPIC -o lib$(LIBNAME).so $^

.c.o:
	$(CC) -c $<

clean:
	rm -rf *.o *~ $(TARGET) lib$(LIBNAME).so
