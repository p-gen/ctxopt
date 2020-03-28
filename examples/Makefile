OBJS    = hello.o example1.o ctxopt.o
BINS    = hello example1
OUT     = hello
CC      = c99
CFLAGS  = -g
LDFLAGS =

all: hello example1

hello: hello.c ctxopt.o
	$(CC) $(CFLAGS) -o hello hello.c ctxopt.o $(LDFLAGS)

example1: example1.c ctxopt.o
	$(CC) $(CFLAGS) -o example1 example1.c ctxopt.o $(LDFLAGS)

ctxopt.o: ../ctxopt.c
	$(CC) $(CFLAGS) -c ../ctxopt.c

clean:
	rm -f $(OBJS) $(BINS)
