TARGET=main
CC=gcc
AR=ar
AROPS=rcs
MAIN=main.c
SRCS=read_database.c question.c answer.c
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
