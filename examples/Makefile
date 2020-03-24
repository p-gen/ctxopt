OBJS    = hello.o ctxopt.o
SOURCE  = hello.c ctxopt.c
HEADER  = ctxopt.h
OUT     = hello
CC      = c99
FLAGS   = -g
LFLAGS  =

all: $(OBJS)
	$(CC) $(OBJS) -o $(OUT) $(LFLAGS)

hello.o: hello.c
	$(CC) -c $(FLAGS) hello.c

ctxopt.o: ../ctxopt.c
	$(CC) -c $(FLAGS) ../ctxopt.c

clean:
	rm -f $(OBJS) $(OUT)
