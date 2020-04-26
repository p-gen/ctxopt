OBJS    = hello.o example1.o example2.o ctxopt.o
BINS    = hello example1 example2
OUT     = hello
CC      = c99
CFLAGS  = -g -O
LDFLAGS =

all: hello example1 example2

hello: hello.c ctxopt.o
	$(CC) $(CFLAGS) -o hello hello.c ctxopt.o $(LDFLAGS)

example1: example1.c ctxopt.o
	$(CC) $(CFLAGS) -o example1 example1.c ctxopt.o $(LDFLAGS)

example2: example2.c ctxopt.o
	$(CC) $(CFLAGS) -o example2 example2.c ctxopt.o $(LDFLAGS)

ctxopt.o: ../ctxopt.c
	$(CC) $(CFLAGS) -c ../ctxopt.c

clean:
	rm -f $(OBJS) $(BINS)
